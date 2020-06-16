#pragma once

#include <vector>
#include <string>

#include "gl.h"

namespace td {

struct shader_var_info {
    GLint location;
    GLenum type;
    GLint size;
    std::string name;
};

void resolve_uniform(GLuint program, GLuint index, GLint name_max_length, shader_var_info& info);
void resolve_attr(GLuint program, GLuint index, GLint name_max_length, shader_var_info& info);

std::vector<shader_var_info> get_active_uniforms(GLuint program);
std::vector<shader_var_info> get_active_attrs(GLuint program);


}