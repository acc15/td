#pragma once

#include <vector>
#include <functional>

#include <td/core/gl.h>
#include <td/core/shader.h>

namespace td {

class program {
public:

    program();

    program(program&& mv) noexcept;
    program(const program& cp) = delete;

    program& operator=(program&& mv) noexcept;
    program& operator=(const program& cp) = delete;

    program&& add(shader&& mv);
    program&& add(shader& cp);
    program&& add(shader_type type, const char* src);
    program&& vertex(const char* src);
    program&& fragment(const char* src);

    GLuint id();
    void rm();

private:

    GLuint _id;
    std::vector<shader> _internals;
    std::vector<std::reference_wrapper<shader>> _externals;

};

}