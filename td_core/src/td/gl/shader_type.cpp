#include <td/gl/shader_type.h>
#include <td/gl/enum.h>

namespace td {

shader_type::shader_type(GLenum value): _value(value) {
}

const char* shader_type::name() const {
    return gl_enum_string(_value);
}

GLenum shader_type::value() const {
    return _value;
}

shader_type::operator GLenum() const {
    return _value;
}

}