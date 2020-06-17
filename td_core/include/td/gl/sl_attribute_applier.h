#pragma once

#include "sl_applier_common.h"

namespace td {

template <typename T>
struct sl_attribute_applier {
};

template <>
struct sl_attribute_applier<GLfloat> {
    typedef GLfloat type;

    static void apply(const td::program& p, const td::sl_var& var, const plain_arithmetic_container_info<type>& value, size_t sl_count) {
        if (var.type.is_float_matrix() || var.type.is_double_matrix()) {
            throw sl_invalid_type("GLfloat", var.type);
        }
        switch (sl_count) {
            case 1:
                if (var.size == 1) {
                    glVertexAttrib1f(var.location, value.ptr[0]);
                } else {
                    glVertexAttrib1fv(var.location, value.ptr);
                }
                break;

            case 2:
                if (var.size == 1) {
                    glVertexAttrib2f(var.location, value.ptr[0], value.ptr[1]);
                } else {
                    glVertexAttrib2fv(var.location, value.ptr);
                }
                break;

            case 3:
                if (var.size == 1) {
                    glVertexAttrib3f(var.location, value.ptr[0], value.ptr[1], value.ptr[2]);
                } else {
                    glVertexAttrib3fv(var.location, value.ptr);
                }
                break;

            case 4:
                if (var.size == 1) {
                    glVertexAttrib4f(var.location, value.ptr[0], value.ptr[1], value.ptr[2], value.ptr[3]);
                } else {
                    glVertexAttrib4fv(var.location, value.ptr);
                }
                break;

            default:
                throw sl_invalid_type("GLfloat", var.type);
        }
    }
};

template <>
struct sl_attribute_applier<GLdouble> {
    typedef GLdouble type;

    static void apply(const td::program& p, const td::sl_var& var, const plain_arithmetic_container_info<type>& value, size_t sl_count) {
        if (var.type.is_float_matrix() || var.type.is_double_matrix()) {
            throw sl_invalid_type("GLdouble", var.type);
        }
        switch (sl_count) {
            case 1:
                if (var.size == 1) {
                    glVertexAttrib1d(var.location, value.ptr[0]);
                } else {
                    glVertexAttrib1dv(var.location, value.ptr);
                }
                break;

            case 2:
                if (var.size == 1) {
                    glVertexAttrib2d(var.location, value.ptr[0], value.ptr[1]);
                } else {
                    glVertexAttrib2dv(var.location, value.ptr);
                }
                break;

            case 3:
                if (var.size == 1) {
                    glVertexAttrib3d(var.location, value.ptr[0], value.ptr[1], value.ptr[2]);
                } else {
                    glVertexAttrib3dv(var.location, value.ptr);
                }
                break;

            case 4:
                if (var.size == 1) {
                    glVertexAttrib4d(var.location, value.ptr[0], value.ptr[1], value.ptr[2], value.ptr[3]);
                } else {
                    glVertexAttrib4dv(var.location, value.ptr);
                }
                break;

            default:
                throw sl_invalid_type("GLdouble", var.type);
        }
    }
};

}

