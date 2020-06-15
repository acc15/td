#pragma once

#include <vector>
#include <unordered_map>
#include <functional>

#include "../gl/gl.h"
#include "shader.h"

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
    program&& attr(const char* name, GLuint index);

    const std::vector<std::reference_wrapper<shader>>& externals() const;
    const std::vector<shader>& internals() const;

    GLuint id();
    void rm();

protected:

    program&& this_mv();

    GLuint _id;

    std::unordered_map<const char*, GLuint> _attrs_locs;
    std::vector<shader> _internals;
    std::vector<std::reference_wrapper<shader>> _externals;

};

}
