#pragma once

#include <fmt/format.h>

#include <td/core/buf.h>
#include <td/gl/gl.h>
#include <td/gl/gl_type.h>
#include <td/core/assert.h>

namespace td {

template <GLenum Type, typename T = void, size_t N = 0>
class bo: public basic_buf<bo<Type, T, N>, T, N> {
public:

    ~bo() {
        rm();
    }

    const bo& bind() const {
        if (_id == 0) {
            throw std::logic_error(fmt::format("Attempt to bind empty buffer object of type {}", Type));
        }
        glBindBuffer(Type, _id);
        return *this;
    }

    bo& apply(GLuint id, GLenum usage = GL_STATIC_DRAW) {
        rm();
        _id = id;
        apply_data(usage);
        return *this;
    }

    bo& apply_usage(GLenum usage = GL_STATIC_DRAW) {
        if (_id == 0) {
            glGenBuffers(1, &_id);
        }
        apply_data(GL_STATIC_DRAW);
        return *this;
    }

    bo& apply() const {
        return apply_usage(GL_STATIC_DRAW);
    }

    void rm() {
        if (_id == 0) {
            return;
        }
        glDeleteBuffers(1, &_id);
        _id = 0;
        _applied_size = 0;
    }

    size_t applied_size() const {
        return _applied_size;
    }

private:

    void apply_data(GLenum usage) {
        glBindBuffer(Type, _id);

        _applied_size = bo::size();
        glBufferData(Type, _applied_size, bo::data(), usage);
        bo::clear();
    }

    size_t _applied_size = 0;
    GLuint _id = 0;

};

template <size_t N = 0> using vbo = bo<GL_ARRAY_BUFFER, void, N>;
template <typename T, size_t N = 0> using ebo = bo<GL_ELEMENT_ARRAY_BUFFER, void, N>;

}
