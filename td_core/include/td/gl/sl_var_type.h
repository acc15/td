#pragma once

#include "gl.h"

namespace td {

class sl_var_type {
public:

    sl_var_type(GLenum value);

    bool is_float_matrix() const;
    bool is_double_matrix() const;
    size_t min_element_count() const;
    const char* name() const;
    GLenum value() const;

    operator GLenum() const;

private:
    GLenum _value;

};

}