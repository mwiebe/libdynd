//
// Copyright (C) 2011-16 DyND Developers
// BSD 2-Clause License, see LICENSE.txt
//

#pragma once

#include <dynd/types/scalar_kind_type.hpp>

namespace dynd {
namespace ndt {

  class bytes_kind_type : public base_type {
  public:
    bytes_kind_type(type_id_t id) : base_type(id, 0, 1, type_flag_symbolic, 0, 0, 0) {}

    bool match(const type &candidate_tp, std::map<std::string, type> &DYND_UNUSED(tp_vars)) const {
      return candidate_tp.get_base_id() == bytes_kind_id;
    }

    void print_data(std::ostream &DYND_UNUSED(o), const char *DYND_UNUSED(arrmeta),
                    const char *DYND_UNUSED(data)) const {
      throw std::runtime_error("cannot print data of bytes_kind_type");
    }

    void print_type(std::ostream &o) const { o << "Bytes"; }

    bool operator==(const base_type &rhs) const { return this == &rhs || rhs.get_id() == bytes_kind_id; }
  };

  template <>
  struct id_of<bytes_kind_type> : std::integral_constant<type_id_t, bytes_kind_id> {};

} // namespace dynd::ndt
} // namespace dynd
