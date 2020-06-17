#include <td/gl/sl_data_type.h>

namespace td {

bool is_float_matrix_type(GLenum type) {
    switch (type) {
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

bool is_double_matrix_type(GLenum type) {
    switch (type) {
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

size_t min_element_count(GLenum type) {
    switch (type) {
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

}