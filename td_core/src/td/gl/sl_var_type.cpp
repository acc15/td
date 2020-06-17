#include <td/gl/sl_var_type.h>
#include <td/gl/enum.h>

namespace td {

sl_var_type::sl_var_type(GLenum value): _value(value) {
}

bool sl_var_type::is_float_matrix() const {
    switch (_value) {
        case GL_FLOAT_MAT2:
        case GL_FLOAT_MAT3:
        case GL_FLOAT_MAT4:
        case GL_FLOAT_MAT2x3:
        case GL_FLOAT_MAT2x4:
        case GL_FLOAT_MAT3x2:
        case GL_FLOAT_MAT3x4:
        case GL_FLOAT_MAT4x2:
        case GL_FLOAT_MAT4x3:
            return true;
        default:
            return false;
    }
}

bool sl_var_type::is_double_matrix() const {
    switch (_value) {
        case GL_DOUBLE_MAT2:
        case GL_DOUBLE_MAT3:
        case GL_DOUBLE_MAT4:
        case GL_DOUBLE_MAT2x3:
        case GL_DOUBLE_MAT2x4:
        case GL_DOUBLE_MAT3x2:
        case GL_DOUBLE_MAT3x4:
        case GL_DOUBLE_MAT4x2:
        case GL_DOUBLE_MAT4x3:
            return true;
        default:
            return false;
    }
}

size_t sl_var_type::min_element_count() const {
    switch (_value) {
        case GL_DOUBLE_VEC2:
        case GL_FLOAT_VEC2:
        case GL_INT_VEC2:
        case GL_UNSIGNED_INT_VEC2:
        case GL_BOOL_VEC2:
            return 2;

        case GL_DOUBLE_VEC3:
        case GL_FLOAT_VEC3:
        case GL_INT_VEC3:
        case GL_UNSIGNED_INT_VEC3:
        case GL_BOOL_VEC3:
            return 3;

        case GL_DOUBLE_VEC4:
        case GL_FLOAT_VEC4:
        case GL_INT_VEC4:
        case GL_UNSIGNED_INT_VEC4:
        case GL_BOOL_VEC4:
        case GL_FLOAT_MAT2:
        case GL_DOUBLE_MAT2:
            return 4;

        case GL_FLOAT_MAT2x3:
        case GL_DOUBLE_MAT2x3:
        case GL_FLOAT_MAT3x2:
        case GL_DOUBLE_MAT3x2:
            return 6;

        case GL_FLOAT_MAT4x2:
        case GL_DOUBLE_MAT4x2:
        case GL_FLOAT_MAT2x4:
        case GL_DOUBLE_MAT2x4:
            return 8;

        case GL_FLOAT_MAT3:
        case GL_DOUBLE_MAT3:
            return 9;

        case GL_FLOAT_MAT4x3:
        case GL_DOUBLE_MAT4x3:
        case GL_FLOAT_MAT3x4:
        case GL_DOUBLE_MAT3x4:
            return 12;

        case GL_FLOAT_MAT4:
        case GL_DOUBLE_MAT4:
            return 16;

        default:
            return 1;
    }
}

const char* sl_var_type::name() const {
    return gl_enum_string(_value);
}

GLenum sl_var_type::value() const {
    return _value;
}

sl_var_type::operator GLenum() const {
    return _value;
}

}