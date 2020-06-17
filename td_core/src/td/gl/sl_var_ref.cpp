#include <td/gl/sl_var_ref.h>
#include <td/gl/program.h>

namespace td {

sl_var_ref::sl_var_ref(GLint location): _location(location), _name(nullptr) {
}

sl_var_ref::sl_var_ref(const char* name): _location(-1), _name(name) {
}

GLint sl_var_ref::uniform_location(const program& p) {
    if (_location == -1 && _name != nullptr) {
        _location = p.uniform_location(_name);
    }
    return _location;
}

GLint sl_var_ref::attribute_location(const program& p) {
    if (_location == -1 && _name != nullptr) {
        _location = p.attribute_location(_name);
    }
    return _location;
}

}