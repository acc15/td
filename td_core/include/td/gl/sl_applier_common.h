#pragma once

#include <stdexcept>
#include <fmt/format.h>

#include "program.h"
#include "../util/plain_arithmetic_container.h"

namespace td {

inline std::invalid_argument sl_invalid_type(const char* c_type, const sl_var_type& sl_type) {
    return std::invalid_argument(fmt::format("invalid input type {} for SL type {}", c_type, sl_type.name()));
}

template <typename Applier>
void sl_apply_value(const td::program& p, const td::sl_var& var, const plain_arithmetic_container_info<typename Applier::type>& value) {
    size_t sl_count = var.type.min_element_count();
    size_t required_count = sl_count * var.size;
    if (value.count < required_count) {
        throw std::invalid_argument(fmt::format(
                "invalid input element count {}, required {} for {} type with size {}",
                value.count, required_count, gl_enum_string(var.type), sl_count));
    }
    Applier::apply(p, var, value, sl_count);
}

}