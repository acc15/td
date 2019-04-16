#pragma once

#include <cstddef>

#include <td/core/gl.h>
#include <td/buffer/buffer.h>


namespace td {

class buffer_object {
public:

    buffer_object(GLenum type);


    buffer& buf();
    buffer_object& data(const void* ptr, size_t size);

    buffer_object& bind();
    buffer_object& unbind();

    buffer_object& apply(GLenum usage);

    GLuint id();
    void rm();
    //bind()

};

}
