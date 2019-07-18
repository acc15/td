#pragma once

#include <cstddef>

#include <td/gl/gl.h>
#include <td/core/buffer.h>
#include <td/gl/gl_type.h>

namespace td {

template <GLenum Type>
class buffer_object {
public:
    ~buffer_object() {
        rm();
    }

    buffer_object& bind() {
        glBindBuffer(Type, id());
        return *this;
    }

    buffer_object& unbind() {
        glBindBuffer(Type, 0);
        return *this;
    }

    GLuint id() {
        if (_id != 0) {
            return _id;
        }
        glGenBuffers(1, &_id);
        return _id;
    }

    template <typename Buffer>
    void apply(Buffer& buf, GLenum usage) {
        _count = buf.count();
        _type = t2gl<typename Buffer::element_type>::value;
        glBufferData(Type, buf.size(), buf.data(), usage);
        if (usage == GL_STATIC_DRAW) {
            buf.free();
        } else {
            buf.reset();
        }
    }

    template <typename Buffer>
    void bind_and_apply(Buffer& buf, GLenum usage) {
        buffer_object<Type>::bind();
        apply<Buffer>(buf, usage);
        buffer_object<Type>::unbind();
    }

    void rm() {
        if (_id == 0) {
            return;
        }
        glDeleteBuffers(1, &_id);
        _id = 0;
    }

    size_t size() const {
        return _count * gl_sizeof(_type);
    }

    size_t count() const {
        return _count;
    }

    GLenum type() const {
        return _type;
    }

private:
    GLuint _id;
    size_t _count;
    GLenum _type;

};

typedef buffer_object<GL_ARRAY_BUFFER> vbo;
typedef buffer_object<GL_ELEMENT_ARRAY_BUFFER> ebo;

}
