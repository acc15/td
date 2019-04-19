#pragma once

#include <cstddef>

#include <td/gl/gl.h>
#include <td/core/buffer.h>

namespace td {

template <GLenum Type>
class buffer_object {
public:
    ~buffer_object() {
        rm();
    }

    void bind() {
        glBindBuffer(Type, id());
        return *this;
    }

    void unbind() {
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

    void rm() {
        if (_id == 0) {
            return;
        }
        glDeleteBuffers(1, &_id);
        _id = 0;
    }

private:
    GLuint _id;

};


// TODO remove buffer (add parameter to apply which accepts buffer)
// TODO remove template parameter BufferType
// TODO define vbo and ebo types

template <typename BufferType, GLenum Type>
class buffer_object_impl: buffer_object<Type> {
public:

    BufferType& buf() {
        return _buf;
    }

    void apply(GLenum usage) {
        glBufferData(Type, _buf.size(), _buf.data(), usage);
        if (usage == GL_STATIC_DRAW) {
            _buf.free();
        } else {
            _buf.reset();
        }
    }

    void bind_and_apply(GLenum usage) {
        buffer_object<Type>::bind();
        apply(usage);
        buffer_object<Type>::unbind();
    }

private:
    BufferType _buf;

};

typedef buffer_object_impl<buffer, GL_ARRAY_BUFFER> vbo;
typedef buffer_object_impl<static_buffer, GL_ARRAY_BUFFER> static_vbo;
typedef buffer_object_impl<static_buffer, GL_ELEMENT_ARRAY_BUFFER> static_ebo;

template <typename IndexType>
using ebo = buffer_object_impl<typed_buffer<IndexType>, GL_ELEMENT_ARRAY_BUFFER>;


}
