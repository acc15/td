#pragma once

#include "gl.h"

namespace td {

bool is_float_matrix_type(GLenum type);
bool is_double_matrix_type(GLenum type);
size_t min_element_count(GLenum type);

}