#pragma once

#include <fmt/format.h>

#include <td/gl/program.h>
#include <td/gl/enum.h>

namespace td {


void throw_invalid_type(const char* type, GLenum gl_type) {
    throw std::invalid_argument(fmt::format("invalid uniform input type: {} for shader type {}", type, gl_enum_string(type));
}

template <typename Applier>
struct generic_element_applier {
    static void apply(typename Applier::value_type* buf, size_t count, GLint loc, GLenum type, GLsizei size) {
        size_t single_count = min_element_count(type);
        size_t required_count = size * single_count;
        if (count < required_count) {
            throw std::invalid_argument(fmt::format(
                    "invalid input count {}, required {} for {} type with size {}",
                    count, required_count, gl_enum_string(type), size));
        }
        Applier::apply(buf, count, loc, type, size, single_count);
    }
};


template <typename T>
struct uniform_applier {
};

template <>
struct uniform_applier<GLuint> {
    typedef GLuint value_type;

    static void apply(GLuint* buf, size_t count, GLint loc, GLenum type, GLsizei size, size_t single_count) {
        if (is_float_matrix_type(type) || is_double_matrix_type(type)) {
            throw_invalid_type("GLuint", gl_enum_string(type));
        }
        switch (single_count) {
            case 1:
                if (size == 1) {
                    glUniform1ui(loc, buf[0]);
                } else {
                    glUniform1uiv(loc, size, buf);
                }
                break;

            case 2:
                if (size == 1) {
                    glUniform2ui(loc, buf[0], buf[1]);
                } else {
                    glUniform2uiv(loc, size, buf);
                }
                break;

            case 3:
                if (size == 1) {
                    glUniform3ui(loc, buf[0], buf[1], buf[2]);
                } else {
                    glUniform3uiv(loc, size, buf);
                }
                break;

            case 4:
                if (size == 1) {
                    glUniform4ui(loc, buf[0], buf[1], buf[2], buf[3]);
                } else {
                    glUniform4uiv(loc, size, buf);
                }
                break;

            default:
                throw_invalid_type("GLuint", gl_enum_string(type));
        }
    }
};

template <>
struct uniform_applier<GLint> {
    static void apply(GLint* buf, size_t count, GLint loc, GLenum type, GLsizei size, size_t single_count) {
        if (is_float_matrix_type(type) || is_double_matrix_type(type)) {
            throw std::invalid_argument(fmt::format("invalid uniform input type GLint for shader type {}", gl_enum_string(type));
        }
        switch (single_count) {
            case 1:
                if (size == 1) {
                    glUniform1i(loc, buf[0]);
                } else {
                    glUniform1iv(loc, size, buf);
                }
                break;

            case 2:
                if (size == 1) {
                    glUniform2i(loc, buf[0], buf[1]);
                } else {
                    glUniform2iv(loc, size, buf);
                }
                break;

            case 3:
                if (size == 1) {
                    glUniform3i(loc, buf[0], buf[1], buf[2]);
                } else {
                    glUniform3iv(loc, size, buf);
                }
                break;

            case 4:
                if (size == 1) {
                    glUniform4i(loc, buf[0], buf[1], buf[2], buf[3]);
                } else {
                    glUniform4iv(loc, size, buf);
                }
                break;

            default:
                throw std::invalid_argument(fmt::format("invalid uniform input type GLint for shader type {}", gl_enum_string(type)));
        }
    }
};

template <>
struct uniform_applier<GLfloat> {
    static void apply(GLfloat* buf, size_t count, GLint loc, GLenum type, GLsizei size, size_t single_count) {
        /*
        if (is_double_matrix_type(type)) {
            throw std::invalid_argument(fmt::format("invalid uniform input type: GLfloat for shader type {}", gl_enum_string(type));
        }
        switch (type) {
            case GL_FLOAT_MAT2:
                glUniformMatrix2fv(loc, size, GL_FALSE, buf);
                break;

                /*
            case GL_FLOAT_MAT3:
            case GL_FLOAT_MAT4:
            case GL_FLOAT_MAT2x3:
            case GL_FLOAT_MAT2x4:
            case GL_FLOAT_MAT3x2:
            case GL_FLOAT_MAT3x4:
            case GL_FLOAT_MAT4x2:
            case GL_FLOAT_MAT4x3:
        }

        switch (single_count) {
            case 1:
                if (size == 1) {
                    glUniform1i(loc, buf[0]);
                } else {
                    glUniform1iv(loc, size, buf);
                }
                break;

            case 2:
                if (size == 1) {
                    glUniform2i(loc, buf[0], buf[1]);
                } else {
                    glUniform2iv(loc, size, buf);
                }
                break;

            case 3:
                if (size == 1) {
                    glUniform3i(loc, buf[0], buf[1], buf[2]);
                } else {
                    glUniform3iv(loc, size, buf);
                }
                break;

            case 4:
                if (size == 1) {
                    glUniform4i(loc, buf[0], buf[1], buf[2], buf[3]);
                } else {
                    glUniform4iv(loc, size, buf);
                }
                break;

            default:
                throw std::invalid_argument(fmt::format("invalid uniform input type: GLint for shader type {}", gl_enum_string(type)));
        }
    }
     */
};

template <>
struct uniform_applier<GLdouble> {
};


class drawer {
public:

    // only external references accepted
    // this is intentional to avoid compiling program every draw operation
    drawer(td::program& p): _p(p) {
        glUseProgram(_p.id());
    }

    template <typename T>
    drawer& uniform(program::var_ref ref, const std::initializer_list<T>& value) {
        return uniform(name, std::vector<T>(value));
    }

    template <typename T>
    drawer& uniform(program::var_ref ref, const T& value) {
        glUniform2d(name.uniform_location(_p), 0.0, 0.0);
        return *this;
    }

    /* overloads for uniform 2, 3 and 4

    template <typename T>
    drawer& uniform(program::var_ref ref, const T& v1, const T& v2);

    template <typename T>
    drawer& uniform(program::var_ref ref, const T& v1, const T& v2, const T& v3);

    template <typename T>
    drawer& uniform(program::var_ref ref, const T& v1, const T& v2, const T& v3, const T& v4);
      */



    template <typename T>
    drawer& attribute(program::var_ref name, const std::initializer_list<T>& value) {
        return attr(name, std::vector<T>(value));
    }

    template <typename T>
    drawer& attribute(program::var_ref name, const T& value) {
        glVertexAttrib1d(name.location(_p), 0.0);
        return *this;
    }

private:
    program& _p;

};


}
