#pragma once

#include "gl.h"

namespace td {

class sl_var_type {
public:

    bool is_float_matrix() const;
    bool is_double_matrix() const;
    size_t min_element_count() const;
    const char* name() const;
    GLenum type() const;

private:
    GLenum _value;

};

}