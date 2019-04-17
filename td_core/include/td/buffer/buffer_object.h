#pragma once

#include <cstddef>

#include <td/gl/gl.h>
#include <td/buffer/buffer.h>


namespace td {

template <typename BufferType, GLenum Type>
class buffer_object {
public:

    ~buffer_object() {
        rm();
    }

    BufferType& buf() {
        return _buf;
    }

    buffer_object& bind() {
        glBindBuffer(Type, id());
        return *this;
    }

    buffer_object& unbind() {
        glBindBuffer(Type, 0);
        return *this;
    }

    buffer_object& apply(GLenum usage) {

        bind();
        glBufferData(Type, _buf.size(), _buf.data(), usage);
        unbind();

        if (usage == GL_STATIC_DRAW) {
            _buf.free();
        } else {
            _buf.reset();
        }
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

    BufferType _buf;

};

typedef buffer_object<buffer, GL_ARRAY_BUFFER> vbo;
typedef buffer_object<static_buffer, GL_ARRAY_BUFFER> static_vbo;
typedef buffer_object<static_buffer, GL_ELEMENT_ARRAY_BUFFER> static_ebo;

template <typename IndexType>
using ebo = buffer_object<typed_buffer<IndexType>, GL_ELEMENT_ARRAY_BUFFER>;


}
