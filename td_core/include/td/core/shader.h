#pragma once

#include "gl.h"

namespace td {

enum shader_type {
    VERTEX = GL_VERTEX_SHADER,
    FRAGMENT = GL_FRAGMENT_SHADER
};


class shader {

public:

    shader(shader_type type, const char* src);
    ~shader();

    const char* src() const;
    shader_type type() const;

    void rm();
    GLuint id();

private:
    GLuint _id;

    const char* _src;
    shader_type _type;

};


}



