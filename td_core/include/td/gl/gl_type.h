#pragma once

#include <stdexcept>

#include <td/gl/gl.h>

namespace td {

template <typename T, GLenum Type>
struct gl_type_def {
    typedef T type;
    constexpr static const GLenum value = Type;
};

template <typename T>
struct gl_type {
    // static_assert(false, "unknown OpenGL type");
};

template <> struct gl_type<GLdouble>: public gl_type_def<GLfloat, GL_DOUBLE> {};
template <> struct gl_type<GLfloat>: public gl_type_def<GLfloat, GL_FLOAT> {};
template <> struct gl_type<GLuint>: public gl_type_def<GLfloat, GL_UNSIGNED_INT> {};
template <> struct gl_type<GLushort>: public gl_type_def<GLfloat, GL_UNSIGNED_SHORT> {};
template <> struct gl_type<GLubyte>: public gl_type_def<GLfloat, GL_UNSIGNED_BYTE> {};
template <> struct gl_type<GLint>: public gl_type_def<GLfloat, GL_INT> {};
template <> struct gl_type<GLshort>: public gl_type_def<GLfloat, GL_SHORT> {};
template <> struct gl_type<GLbyte>: public gl_type_def<GLfloat, GL_BYTE> {};

inline size_t gl_sizeof(GLenum type) {
    switch (type) {
        case GL_UNSIGNED_INT: return sizeof(GLuint);
        case GL_INT: return sizeof(GLint);
        case GL_UNSIGNED_SHORT: return sizeof(GLushort);
        case GL_SHORT: return sizeof(GLshort);
        case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
        case GL_BYTE: return sizeof(GLbyte);
        case GL_FLOAT: return sizeof(GLfloat);
        case GL_DOUBLE: return sizeof(GLdouble);
        case GL_FIXED: return sizeof(GLfixed);
        case GL_HALF_FLOAT: return sizeof(GLhalf);
        default:
            throw std::invalid_argument("Unknown OpenGL type constant " + std::to_string(type));

    }
}

}