#pragma once

#include <cstddef>

#include <td/gl/gl.h>
#include <td/buffer/buffer.h>


namespace td {

template <typename BufferType>
class buffer_object {
public:

    buffer_object(GLenum type): _type(type) {
    }

    ~buffer_object() {
        rm();
    }

    BufferType& buf() {
        return _buf;
    }

    /*
    buffer_object& data(const void* ptr, size_t size) {
        _buf.free();
        _ptr = ptr;
        _size = size;
    }*/

    buffer_object& bind() {
        glBindBuffer(_type, id());
        return *this;
    }

    buffer_object& unbind() {
        glBindBuffer(_type, 0);
        return *this;
    }

    buffer_object& apply(GLenum usage) {

        bind();
        glBufferData(_type, _buf.size(), _buf.data(), usage);
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
    GLenum _type;
    GLuint _id;

    BufferType _buf;

};

typedef buffer_object<buffer> vbo;

template <typename IndexType>
using ebo = buffer_object<typed_buffer<IndexType>>;


}
