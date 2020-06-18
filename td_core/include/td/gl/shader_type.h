#pragma once

#include "gl.h"

namespace td {

class shader_type {
public:
    shader_type(GLenum value);
    const char* name() const;
    GLenum value() const;
    operator GLenum() const;

private:
    GLenum _value;

};

}

