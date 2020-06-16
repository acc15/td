#include <td/gl/shader_var_info.h>

namespace td {

void resolve_uniform(GLuint program, GLuint index, GLint name_max_length, shader_var_info& info) {
    info.location = index;

    GLsizei name_length;
    info.name.resize(name_max_length);
    glGetActiveUniform(program, index, info.name.size(), &name_length, &info.size, &info.type, info.name.data());
    info.name.resize(name_length);
}


void resolve_attr(GLuint program, GLuint index, GLint name_max_length, shader_var_info& info) {
    info.location = index;

    GLsizei name_length;
    info.name.resize(name_max_length);
    glGetActiveAttrib(program, index, info.name.size(), &name_length, &info.size, &info.type, info.name.data());
    info.name.resize(name_length);
}

std::vector<shader_var_info> get_active_uniforms(GLuint program) {
    GLint count, max_len;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);
    glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_len);

    std::vector<shader_var_info> v(count);
    for (size_t i = 0; i < v.size(); i++) {
        resolve_uniform(program, i, max_len, v[i]);
    }
    return v;
}

std::vector<shader_var_info> get_active_attrs(GLuint program) {
    GLint count, max_len;
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &count);
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max_len);

    std::vector<shader_var_info> v(count);
    for (size_t i = 0; i < v.size(); i++) {
        resolve_attr(program, i, max_len, v[i]);
    }
    return v;
}


}