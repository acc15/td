#pragma once

#include <td/core/buf.h>
#include <td/gl/gl.h>
#include <td/gl/gl_type.h>

namespace td {


template <GLenum Type, typename T = void, size_t N = 0>
class bo: public basic_buf<bo<Type, T, N>, T, N> {
public:

    ~bo() {
        rm();
    }

    bo& bind() const {
        glBindBuffer(Type, _id);
    }

    bo& apply(GLuint id, GLenum usage = GL_STATIC_DRAW) {
        rm();
        _id = id;
        apply_data(usage);
        return *this;
    }

    bo& apply_usage(GLenum usage = GL_STATIC_DRAW) {
        rm();
        glGenBuffers(1, &_id);
        apply_data(GL_STATIC_DRAW);
        return *this;
    }

    bo& apply() const {
        return apply_usage(GL_STATIC_DRAW);
    }

    void rm() {
        if (_id != 0) {
            glDeleteBuffers(1, &_id);
            _id = 0;
        }
    }

private:

    void apply_data(GLenum usage) {
        glBindBuffer(Type, _id);
        glBufferData(Type, bo::size(), bo::data(), usage);
    }

    GLuint _id = 0;

};

template <size_t N = 0> using vbo = bo<GL_ARRAY_BUFFER, void, N>;
template <typename T, size_t N = 0> using ebo = bo<GL_ELEMENT_ARRAY_BUFFER, void, N>;

}
