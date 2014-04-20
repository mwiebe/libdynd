//
// Copyright (C) 2011-14 Mark Wiebe, DyND Developers
// BSD 2-Clause License, see LICENSE.txt
//

#include <dynd/kernels/ckernel_deferred.hpp>
#include <dynd/kernels/assignment_kernels.hpp>
#include <dynd/kernels/ckernel_common_functions.hpp>
#include <dynd/kernels/expr_kernels.hpp>
#include <dynd/types/expr_type.hpp>
#include <dynd/types/base_struct_type.hpp>
#include <dynd/types/property_type.hpp>
#include <dynd/type.hpp>

using namespace std;
using namespace dynd;

namespace {

////////////////////////////////////////////////////////////////
// Structure and functions for the unary assignment as a deferred ckernel

struct unary_assignment_ckernel_deferred_data {
    ndt::type data_types[3];
    assign_error_mode errmode;
};

static void delete_unary_assignment_ckernel_deferred_data(void *self_data_ptr)
{
    unary_assignment_ckernel_deferred_data *data =
                    reinterpret_cast<unary_assignment_ckernel_deferred_data *>(self_data_ptr);
    delete data;
}

static intptr_t instantiate_unary_assignment_ckernel(
    void *self_data_ptr, dynd::ckernel_builder *out_ckb, intptr_t ckb_offset,
    const char *const *dynd_metadata, uint32_t kerntype,
    const eval::eval_context *ectx)
{
    unary_assignment_ckernel_deferred_data *data =
                    reinterpret_cast<unary_assignment_ckernel_deferred_data *>(self_data_ptr);
    return make_assignment_kernel(out_ckb, ckb_offset,
                    data->data_types[0], dynd_metadata[0],
                    data->data_types[2], dynd_metadata[1],
                    (kernel_request_t)kerntype, data->errmode, ectx);
}

static intptr_t instantiate_adapted_expr_assignment_ckernel(
    void *self_data_ptr, dynd::ckernel_builder *out_ckb, intptr_t ckb_offset,
    const char *const *dynd_metadata, uint32_t kerntype,
    const eval::eval_context *ectx)
{
    unary_assignment_ckernel_deferred_data *data =
                    reinterpret_cast<unary_assignment_ckernel_deferred_data *>(self_data_ptr);
    ckb_offset = kernels::wrap_unary_as_expr_ckernel(out_ckb, ckb_offset, (kernel_request_t)kerntype);
    return make_assignment_kernel(out_ckb, ckb_offset,
                    data->data_types[0], dynd_metadata[0],
                    data->data_types[2], dynd_metadata[1],
                    (kernel_request_t)kerntype, data->errmode, ectx);
}

////////////////////////////////////////////////////////////////
// Structure and functions for the expr as a deferred ckernel

struct expr_ckernel_deferred_data {
    assign_error_mode errmode;
    const dynd::expr_type *expr_type;
    size_t data_types_size;
    ndt::type data_types[1];
};

static void delete_expr_ckernel_deferred_data(void *self_data_ptr)
{
    expr_ckernel_deferred_data *data =
                    reinterpret_cast<expr_ckernel_deferred_data *>(self_data_ptr);
    base_type_xdecref(data->expr_type);
    ndt::type *data_types = &data->data_types[0];
    for (size_t i = 0; i < data->data_types_size; ++i) {
        // Reset all the types to NULL
        data_types[i] = ndt::type();
    }
    // Call the destructor and free the memory
    data->~expr_ckernel_deferred_data();
    free(data);
}

static intptr_t
instantiate_expr_ckernel(void *self_data_ptr, dynd::ckernel_builder *out_ckb,
                         intptr_t ckb_offset, const char *const *dynd_metadata,
                         uint32_t kerntype, const eval::eval_context *ectx)
{
    expr_ckernel_deferred_data *data =
                    reinterpret_cast<expr_ckernel_deferred_data *>(self_data_ptr);
    const expr_kernel_generator& kgen = data->expr_type->get_kgen();
    return kgen.make_expr_kernel(out_ckb, ckb_offset,
                    data->data_types[0], dynd_metadata[0],
                    data->data_types_size - 1, &data->data_types[0] + 1,
                    const_cast<const char **>(dynd_metadata) + 1,
                    (kernel_request_t)kerntype, ectx);
}



} // anonymous namespace

void dynd::make_ckernel_deferred_from_assignment(
                const ndt::type& dst_tp, const ndt::type& src_tp, const ndt::type& src_expr_tp,
                deferred_ckernel_funcproto_t funcproto,
                assign_error_mode errmode, ckernel_deferred& out_ckd)
{
    if (src_tp.operand_type() != src_expr_tp.operand_type()) {
        stringstream ss;
        ss << "make_ckernel_deferred_from_assignment: src_tp " << src_tp;
        ss << " and src_expr_tp " << src_expr_tp;
        ss << " must have matching operand types";
        throw type_error(ss.str());
    }
    memset(&out_ckd, 0, sizeof(ckernel_deferred));
    if (funcproto == unary_operation_funcproto) {
        // Since a unary operation was requested, it's a straightforward unary assignment ckernel
        unary_assignment_ckernel_deferred_data *data = new unary_assignment_ckernel_deferred_data;
        out_ckd.data_ptr = data;
        out_ckd.free_func = &delete_unary_assignment_ckernel_deferred_data;
        data->data_types[0] = dst_tp;
        data->data_types[1] = src_tp;
        data->data_types[2] = src_expr_tp;
        data->errmode = errmode;
        out_ckd.instantiate_func = &instantiate_unary_assignment_ckernel;
        out_ckd.ckernel_funcproto = unary_operation_funcproto;
        out_ckd.data_types_size = 2;
        out_ckd.data_dynd_types = data->data_types;
    } else if (funcproto == expr_operation_funcproto) {
        if (src_tp.get_type_id() == expr_type_id && (&src_tp == &src_expr_tp)) {
            const expr_type *etp = src_tp.tcast<expr_type>();
            const base_struct_type *operands_type = etp->get_operand_type().tcast<base_struct_type>();
            const ndt::type *operand_types = operands_type->get_field_types();
            // Expose the expr type's expression
            intptr_t nargs = operands_type->get_field_count();
            size_t sizeof_data_mem = sizeof(expr_ckernel_deferred_data) + sizeof(void *) * nargs;
            void *data_mem = malloc(sizeof_data_mem);
            memset(data_mem, 0, sizeof_data_mem);
            expr_ckernel_deferred_data *data = reinterpret_cast<expr_ckernel_deferred_data *>(data_mem);
            out_ckd.data_ptr = data;
            out_ckd.free_func = &delete_expr_ckernel_deferred_data;
            data->data_types_size = nargs + 1;
            ndt::type *data_types_arr = &data->data_types[0];
            data_types_arr[0] = dst_tp;
            for (intptr_t i = 0; i < nargs; ++i) {
                // Dereference the pointer type in each field
                const pointer_type *field_ptr_type = operand_types[i].tcast<pointer_type>();
                data_types_arr[i+1] = field_ptr_type->get_target_type();
            }
            data->expr_type = static_cast<const expr_type *>(ndt::type(etp, true).release());
            data->errmode = errmode;
            out_ckd.instantiate_func = &instantiate_expr_ckernel;
            out_ckd.ckernel_funcproto = expr_operation_funcproto;
            out_ckd.data_types_size = nargs + 1;
            out_ckd.data_dynd_types = data->data_types;
        } else {
            // Adapt the assignment to an expr kernel
            unary_assignment_ckernel_deferred_data *data = new unary_assignment_ckernel_deferred_data;
            out_ckd.data_ptr = data;
            out_ckd.free_func = &delete_unary_assignment_ckernel_deferred_data;
            data->data_types[0] = dst_tp;
            data->data_types[1] = src_tp;
            data->data_types[2] = src_expr_tp;
            data->errmode = errmode;
            out_ckd.instantiate_func = &instantiate_adapted_expr_assignment_ckernel;
            out_ckd.ckernel_funcproto = expr_operation_funcproto;
            out_ckd.data_types_size = 2;
            out_ckd.data_dynd_types = data->data_types;
        }
    } else {
        stringstream ss;
        ss << "unrecognized ckernel function prototype enum value " << funcproto;
        throw runtime_error(ss.str());
    }
}


void dynd::make_ckernel_deferred_from_property(const ndt::type& tp, const std::string& propname,
                deferred_ckernel_funcproto_t funcproto,
                assign_error_mode errmode, ckernel_deferred& out_ckd)
{
    ndt::type prop_tp = ndt::make_property(tp, propname);
    ndt::type dst_tp = prop_tp.value_type();
    make_ckernel_deferred_from_assignment(dst_tp, tp, prop_tp, funcproto, errmode, out_ckd);
}
