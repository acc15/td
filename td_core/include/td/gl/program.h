#pragma once

#include <vector>
#include <unordered_map>
#include <functional>

#include "gl.h"
#include "shader.h"

namespace td {

class program {
public:

    struct var_info {
        GLint location;
        GLuint index;
        GLenum type;
        GLint size;
        const std::string& name;
    };

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

    GLint uniform_location(const char* name) const;
    GLuint attribute_location(const char* name) const;
    const var_info& uniform(GLint location) const;
    const var_info& uniform(const char* name) const;
    const var_info& attribute(GLuint index) const;
    const var_info& attribute(const char* name) const;
    const std::vector<var_info>& uniforms() const;
    const std::vector<var_info>& attributes() const;

    GLuint id();
    void rm();

protected:

    void populate_uniforms(GLuint id);
    void populate_attributes(GLuint id);

    program&& this_mv();

    GLuint _id;

    std::unordered_map<std::string, GLint> _uniform_locs;
    std::unordered_map<std::string, GLint> _attr_locs;
    std::vector<var_info> _uniforms;
    std::vector<var_info> _attrs;
    std::unordered_map<GLint, var_info*> _uniform_map;
    std::unordered_map<GLint, var_info*> _attr_map;

    std::unordered_map<const char*, GLuint> _bind_attrs;
    std::vector<shader> _internals;
    std::vector<std::reference_wrapper<shader>> _externals;

};

}
