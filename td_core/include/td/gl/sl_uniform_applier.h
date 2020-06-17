#pragma once

#include "gl.h"
#include "program.h"
#include "sl_applier_common.h"

namespace td {

template <typename T>
struct sl_uniform_applier {
};

template <>
struct sl_uniform_applier<GLuint> {
    typedef GLuint type;

    static void apply(const td::program& p, const td::sl_var& var, const plain_arithmetic_container_info<type>& value, size_t sl_count) {
        if (var.type.is_float_matrix() || var.type.is_double_matrix()) {
            throw sl_invalid_type("GLuint", var.type);
        }
        switch (sl_count) {
            case 1:
                if (var.size == 1) {
                    glUniform1ui(var.location, value.ptr[0]);
                } else {
                    glUniform1uiv(var.location, var.size, value.ptr);
                }
                break;

            case 2:
                if (var.size == 1) {
                    glUniform2ui(var.location, value.ptr[0], value.ptr[1]);
                } else {
                    glUniform2uiv(var.location, var.size, value.ptr);
                }
                break;

            case 3:
                if (var.size == 1) {
                    glUniform3ui(var.location, value.ptr[0], value.ptr[1], value.ptr[2]);
                } else {
                    glUniform3uiv(var.location, var.size, value.ptr);
                }
                break;

            case 4:
                if (var.size == 1) {
                    glUniform4ui(var.location, value.ptr[0], value.ptr[1], value.ptr[2], value.ptr[3]);
                } else {
                    glUniform4uiv(var.location, var.size, value.ptr);
                }
                break;

            default:
                throw sl_invalid_type("GLuint", var.type);
        }
    }
};

template <>
struct sl_uniform_applier<GLint> {
    typedef GLint type;

    static void apply(const td::program& p, const td::sl_var& var, const plain_arithmetic_container_info<type>& value, size_t sl_count) {
        if (var.type.is_float_matrix() || var.type.is_double_matrix()) {
            throw sl_invalid_type("GLint", var.type);
        }
        switch (sl_count) {
            case 1:
                if (var.size == 1) {
                    glUniform1i(var.location, value.ptr[0]);
                } else {
                    glUniform1iv(var.location, var.size, value.ptr);
                }
                break;

            case 2:
                if (var.size == 1) {
                    glUniform2i(var.location, value.ptr[0], value.ptr[1]);
                } else {
                    glUniform2iv(var.location, var.size, value.ptr);
                }
                break;

            case 3:
                if (var.size == 1) {
                    glUniform3i(var.location, value.ptr[0], value.ptr[1], value.ptr[2]);
                } else {
                    glUniform3iv(var.location, var.size, value.ptr);
                }
                break;

            case 4:
                if (var.size == 1) {
                    glUniform4i(var.location, value.ptr[0], value.ptr[1], value.ptr[2], value.ptr[3]);
                } else {
                    glUniform4iv(var.location, var.size, value.ptr);
                }
                break;

            default:
                throw sl_invalid_type("GLint", var.type);
        }
    }
};

template <>
struct sl_uniform_applier<GLfloat> {
    typedef GLfloat type;

    static void apply(const td::program& p, const td::sl_var& var, const plain_arithmetic_container_info<type>& value, size_t sl_count) {
        if (var.type.is_double_matrix()) {
            throw sl_invalid_type("GLfloat", var.type);
        }
        switch (var.type.value()) {
            case GL_FLOAT_MAT2:
                glUniformMatrix2fv(var.location, var.size, GL_FALSE, value.ptr);
                return;
            case GL_FLOAT_MAT3:
                glUniformMatrix3fv(var.location, var.size, GL_FALSE, value.ptr);
                return;
            case GL_FLOAT_MAT4:
                glUniformMatrix4fv(var.location, var.size, GL_FALSE, value.ptr);
                return;
            case GL_FLOAT_MAT2x3:
                glUniformMatrix2x3fv(var.location, var.size, GL_FALSE, value.ptr);
                return;
            case GL_FLOAT_MAT2x4:
                glUniformMatrix2x4fv(var.location, var.size, GL_FALSE, value.ptr);
                return;
            case GL_FLOAT_MAT3x2:
                glUniformMatrix3x2fv(var.location, var.size, GL_FALSE, value.ptr);
                return;
            case GL_FLOAT_MAT3x4:
                glUniformMatrix3x4fv(var.location, var.size, GL_FALSE, value.ptr);
                return;
            case GL_FLOAT_MAT4x2:
                glUniformMatrix4x2fv(var.location, var.size, GL_FALSE, value.ptr);
                return;
            case GL_FLOAT_MAT4x3:
                glUniformMatrix4x3fv(var.location, var.size, GL_FALSE, value.ptr);
                return;
        }
        switch (sl_count) {
            case 1:
                if (var.size == 1) {
                    glUniform1f(var.location, value.ptr[0]);
                } else {
                    glUniform1fv(var.location, var.size, value.ptr);
                }
                break;

            case 2:
                if (var.size == 1) {
                    glUniform2f(var.location, value.ptr[0], value.ptr[1]);
                } else {
                    glUniform2fv(var.location, var.size, value.ptr);
                }
                break;

            case 3:
                if (var.size == 1) {
                    glUniform3f(var.location, value.ptr[0], value.ptr[1], value.ptr[2]);
                } else {
                    glUniform3fv(var.location, var.size, value.ptr);
                }
                break;

            case 4:
                if (var.size == 1) {
                    glUniform4f(var.location, value.ptr[0], value.ptr[1], value.ptr[2], value.ptr[3]);
                } else {
                    glUniform4fv(var.location, var.size, value.ptr);
                }
                break;

            default:
                throw sl_invalid_type("GLfloat", var.type);
        }
    }
};

template <>
struct sl_uniform_applier<GLdouble> {
    typedef GLdouble type;

    static void apply(const td::program& p, const td::sl_var& var, const plain_arithmetic_container_info<type>& value, size_t sl_count) {
        if (var.type.is_float_matrix()) {
            throw sl_invalid_type("GLdouble", var.type);
        }
        switch (var.type.value()) {
            case GL_FLOAT_MAT2:
                glUniformMatrix2dv(var.location, var.size, GL_FALSE, value.ptr);
                return;
            case GL_FLOAT_MAT3:
                glUniformMatrix3dv(var.location, var.size, GL_FALSE, value.ptr);
                return;
            case GL_FLOAT_MAT4:
                glUniformMatrix4dv(var.location, var.size, GL_FALSE, value.ptr);
                return;
            case GL_FLOAT_MAT2x3:
                glUniformMatrix2x3dv(var.location, var.size, GL_FALSE, value.ptr);
                return;
            case GL_FLOAT_MAT2x4:
                glUniformMatrix2x4dv(var.location, var.size, GL_FALSE, value.ptr);
                return;
            case GL_FLOAT_MAT3x2:
                glUniformMatrix3x2dv(var.location, var.size, GL_FALSE, value.ptr);
                return;
            case GL_FLOAT_MAT3x4:
                glUniformMatrix3x4dv(var.location, var.size, GL_FALSE, value.ptr);
                return;
            case GL_FLOAT_MAT4x2:
                glUniformMatrix4x2dv(var.location, var.size, GL_FALSE, value.ptr);
                return;
            case GL_FLOAT_MAT4x3:
                glUniformMatrix4x3dv(var.location, var.size, GL_FALSE, value.ptr);
                return;
        }
        switch (sl_count) {
            case 1:
                if (var.size == 1) {
                    glUniform1d(var.location, value.ptr[0]);
                } else {
                    glUniform1dv(var.location, var.size, value.ptr);
                }
                break;

            case 2:
                if (var.size == 1) {
                    glUniform2d(var.location, value.ptr[0], value.ptr[1]);
                } else {
                    glUniform2dv(var.location, var.size, value.ptr);
                }
                break;

            case 3:
                if (var.size == 1) {
                    glUniform3d(var.location, value.ptr[0], value.ptr[1], value.ptr[2]);
                } else {
                    glUniform3dv(var.location, var.size, value.ptr);
                }
                break;

            case 4:
                if (var.size == 1) {
                    glUniform4d(var.location, value.ptr[0], value.ptr[1], value.ptr[2], value.ptr[3]);
                } else {
                    glUniform4dv(var.location, var.size, value.ptr);
                }
                break;

            default:
                throw sl_invalid_type("GLdouble", var.type);
        }
    }
};

}
