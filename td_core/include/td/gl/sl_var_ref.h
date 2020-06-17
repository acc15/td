#pragma once

#include "gl.h"

namespace td {

class program;

class sl_var_ref {
public:
    sl_var_ref(GLint location);
    sl_var_ref(const char* name);

    GLint uniform_location(const program& p);
    GLint attribute_location(const program& p);

private:
    GLint _location;
    const char* _name;

};

}

