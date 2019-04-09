#pragma once

#include <vector>
#include <functional>

#include <td/core/gl.h>
#include <td/core/shader.h>

class program {
public:

    program();

    program(program&& mv) noexcept;
    program(const program& cp) = delete;

    program& operator=(program&& mv) noexcept;
    program& operator=(const program& cp) = delete;

    program&& add(td::shader&& mv);
    program&& add(td::shader& cp);

    GLuint id();
    void rm();

private:

    GLuint _id;
    std::vector<td::shader> _internals;
    std::vector<std::reference_wrapper<td::shader>> _externals;

};
