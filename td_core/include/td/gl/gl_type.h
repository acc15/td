#pragma once

#include <stdexcept>
#include <fmt/format.h>
#include <td/gl/gl.h>

namespace td {

template <typename T, GLenum Type>
struct gl_type_def {
    typedef T type;
    constexpr static const GLenum value = Type;
};

template <typename T>
struct t2gl {
    // static_assert(false, "unknown OpenGL type");
};
template <> struct t2gl<GLfloat>: public gl_type_def<GLfloat, GL_FLOAT> {};
template <> struct t2gl<GLhalf>: public gl_type_def<GLhalf, GL_HALF_FLOAT> {};
template <> struct t2gl<GLdouble>: public gl_type_def<GLdouble, GL_DOUBLE> {};
template <> struct t2gl<GLbyte>: public gl_type_def<GLbyte, GL_BYTE> {};
template <> struct t2gl<GLubyte>: public gl_type_def<GLubyte, GL_UNSIGNED_BYTE> {};
template <> struct t2gl<GLshort>: public gl_type_def<GLshort, GL_SHORT> {};
// template <> struct t2gl<GLushort>: public gl_type_def<GLushort, GL_UNSIGNED_SHORT> {};
template <> struct t2gl<GLint>: public gl_type_def<GLint, GL_INT> {};
template <> struct t2gl<GLuint>: public gl_type_def<GLuint, GL_UNSIGNED_INT> {};
// template <> struct t2gl<GLfixed>: public gl_type_def<GLfixed, GL_FIXED> {};

template <GLenum type> struct gl2t {};
template <> struct gl2t<GL_FLOAT>: public gl_type_def<GLfloat, GL_FLOAT> {};
template <> struct gl2t<GL_HALF_FLOAT>: public gl_type_def<GLhalf, GL_HALF_FLOAT> {};
template <> struct gl2t<GL_DOUBLE>: public gl_type_def<GLdouble, GL_DOUBLE> {};
template <> struct gl2t<GL_BYTE>: public gl_type_def<GLbyte, GL_BYTE> {};
template <> struct gl2t<GL_UNSIGNED_BYTE>: public gl_type_def<GLubyte, GL_UNSIGNED_BYTE> {};
template <> struct gl2t<GL_SHORT>: public gl_type_def<GLshort, GL_SHORT> {};
template <> struct gl2t<GL_UNSIGNED_SHORT>: public gl_type_def<GLushort, GL_UNSIGNED_SHORT> {};
template <> struct gl2t<GL_INT>: public gl_type_def<GLint, GL_INT> {};
template <> struct gl2t<GL_UNSIGNED_INT>: public gl_type_def<GLuint, GL_UNSIGNED_INT> {};
template <> struct gl2t<GL_FIXED>: public gl_type_def<GLfixed, GL_FIXED> {};

constexpr inline size_t gl_sizeof(GLenum type) {
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
            throw std::invalid_argument(fmt::format("Unknown OpenGL type constant {}", type));
    }
}

}
