#pragma once

#include <vector>
#include <unordered_map>
#include <functional>

#include "gl.h"
#include "shader.h"
#include "shader_var_info.h"

namespace td {

class program {
public:

    program();
    ~program();

    program(program&& mv) noexcept;
    program& operator=(program&& mv) noexcept;

    program&& add(shader&& mv);
    program&& add(shader& cp);
    program&& add(shader_type type, const char* src);
    program&& vertex(const char* src);
    program&& fragment(const char* src);
    program&& bind_attr(const char* name, GLuint index);

    const std::vector<std::reference_wrapper<shader>>& externals() const;
    const std::vector<shader>& internals() const;

    /*
    GLint uniform_location(const char* name) const;
    GLuint attribute_location(const char* name) const;

    const shader_var_info& uniform(GLint location) const;
    const shader_var_info& uniform(const char* name) const;
    const shader_var_info& attribute(GLuint index) const;
    const shader_var_info& attribute(const char* name) const;
     */

    GLuint id();
    void rm();

protected:

    program&& this_mv();

    GLuint _id;

    /*
    std::unordered_map<const char*, GLint> _uniform_locs;
    std::unordered_map<const char*, GLuint> _attr_locs;
    std::unordered_map<GLint, shader_var_info> _uniforms;
    std::unordered_map<GLuint, shader_var_info> _attrs;
     */

    std::unordered_map<const char*, GLuint> _bind_attrs;
    std::vector<shader> _internals;
    std::vector<std::reference_wrapper<shader>> _externals;

};

}
