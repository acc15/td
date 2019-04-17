#pragma once

#include "td/gl/gl.h"

namespace td {


class shader {

public:

    shader(GLenum type, const char* src);
    ~shader();

    const char* src() const;
    GLenum type() const;

    void rm();
    GLuint id();

private:
    GLuint _id;

    const char* _src;
    GLenum _type;

};


}



