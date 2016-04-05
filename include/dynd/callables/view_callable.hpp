//
// Copyright (C) 2011-16 DyND Developers
// BSD 2-Clause License, see LICENSE.txt
//

#pragma once

#include <dynd/callables/default_instantiable_callable.hpp>
#include <dynd/kernels/view_kernel.hpp>

namespace dynd {
namespace nd {

  class view_callable : public base_callable {
  public:
    view_callable() : base_callable(ndt::type("(Any) -> Any")) {}

    ndt::type resolve(base_callable *DYND_UNUSED(caller), char *DYND_UNUSED(data), call_graph &cg,
                      const ndt::type &DYND_UNUSED(dst_tp), size_t DYND_UNUSED(nsrc), const ndt::type *src_tp,
                      size_t DYND_UNUSED(nkwd), const array *DYND_UNUSED(kwds),
                      const std::map<std::string, ndt::type> &DYND_UNUSED(tp_vars)) {
      cg.push_back([](call_node *&DYND_UNUSED(node), kernel_builder * ckb, kernel_request_t kernreq,
                      const char *DYND_UNUSED(dst_arrmeta), size_t DYND_UNUSED(nsrc),
                      const char *const *DYND_UNUSED(src_arrmeta)) { ckb->emplace_back<view_kernel>(kernreq); });

      return src_tp[0];
    }
  };

} // namespace dynd::nd
} // namespace dynd
