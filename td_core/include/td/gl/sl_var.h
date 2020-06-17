#pragma once

#include <string>

#include "gl.h"
#include "sl_var_type.h"

namespace td {

struct sl_var {
    GLint location;
    GLuint index;
    sl_var_type type;
    GLint size;
    const std::string& name;
};

}

