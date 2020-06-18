#pragma once

#include <stdexcept>
#include <fmt/format.h>

#include "gl.h"
#include "shader_type.h"

namespace td {

class shader {

public:
    shader(const shader_type&  type, const char* src);
    ~shader();

    const char* src() const;
    const shader_type& type() const;

    void rm();
    GLuint compile();

    GLuint id() const;

private:
    GLuint _id;

    const char* _src;
    shader_type _type;

};


}



