#pragma once

#include <vector>
#include <unordered_map>
#include <functional>

#include "gl.h"
#include "shader.h"
#include "sl_var_ref.h"
#include "sl_var.h"

namespace td {

class program {
public:
    program();
    ~program();

    program(program&& mv) noexcept;
    program& operator=(program&& mv) noexcept;

    program&& add(shader&& mv);
    program&& add(shader& cp);
    program&& add(const shader_type& type, const char* src);
    program&& vertex(const char* src);
    program&& fragment(const char* src);
    program&& bind_attr(const char* name, GLuint index);

    const std::vector<std::reference_wrapper<shader>>& externals() const;
    const std::vector<shader>& internals() const;

    GLint uniform_location(const char* name) const;
    GLuint attribute_location(const char* name) const;
    const sl_var& uniform(sl_var_ref ref) const;
    const sl_var& attribute(sl_var_ref ref) const;
    const std::vector<sl_var>& uniforms() const;
    const std::vector<sl_var>& attributes() const;

    GLuint link();
    void rm();

    GLuint id() const;

protected:

    void populate_uniforms(GLuint id);
    void populate_attributes(GLuint id);

    program&& this_mv();

    GLuint _id;

    std::unordered_map<std::string, GLint> _uniform_locs;
    std::unordered_map<std::string, GLint> _attr_locs;
    std::vector<sl_var> _uniforms;
    std::vector<sl_var> _attrs;
    std::unordered_map<GLint, sl_var*> _uniform_map;
    std::unordered_map<GLint, sl_var*> _attr_map;

    std::unordered_map<const char*, GLuint> _bind_attrs;
    std::vector<shader> _internals;
    std::vector<std::reference_wrapper<shader>> _externals;

};

}
