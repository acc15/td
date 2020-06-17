#pragma once

#include <fmt/format.h>

#include <td/gl/program.h>
#include <td/gl/sl_data_type.h>
#include <td/gl/enum.h>

#include <td/util/plain_arithmetic_container.h>

namespace td {



std::invalid_argument sl_invalid_type(const char* c_type, GLenum gl_type) {
    const char* gl_type_string = gl_enum_string(gl_type);
    return std::invalid_argument(fmt::format("invalid input type {} for shader type {}", c_type, gl_type_string));
}

template <typename Applier>
void sl_apply_value(const td::program& p, const td::program::var_info& var, const plain_arithmetic_container_info<typename Applier::type>& value) {
    size_t sl_count = min_element_count(var.type);
    size_t required_count = sl_count * var.size;
    if (value.count < required_count) {
        throw std::invalid_argument(fmt::format(
                "invalid input element count {}, required {} for {} type with size {}",
                value.count, required_count, gl_enum_string(var.type), sl_count));
    }
    Applier::apply(e_ptr, sl_loc, sl_type, sl_array_len, sl_count);
}


template <typename T>
struct sl_uniform_applier {
};

template <>
struct sl_uniform_applier<GLuint> {
    typedef GLuint value_type;

    static void apply(const td::program& p, const td::program::var_info& var, const plain_arithmetic_container_info<GLuint>& value) {
        if (is_float_matrix_type(var.type) || is_double_matrix_type(var.type)) {
            throw sl_invalid_type("GLuint", var.type);
        }
        switch (var) {
            case 1:
                if (sl_array_len == 1) {
                    glUniform1ui(sl_loc, e_ptr[0]);
                } else {
                    glUniform1uiv(sl_loc, sl_array_len, e_ptr);
                }
                break;

            case 2:
                if (sl_array_len == 1) {
                    glUniform2ui(sl_loc, e_ptr[0], e_ptr[1]);
                } else {
                    glUniform2uiv(sl_loc, sl_array_len, e_ptr);
                }
                break;

            case 3:
                if (sl_array_len == 1) {
                    glUniform3ui(sl_loc, e_ptr[0], e_ptr[1], e_ptr[2]);
                } else {
                    glUniform3uiv(sl_loc, sl_array_len, e_ptr);
                }
                break;

            case 4:
                if (sl_array_len == 1) {
                    glUniform4ui(sl_loc, e_ptr[0], e_ptr[1], e_ptr[2], e_ptr[3]);
                } else {
                    glUniform4uiv(sl_loc, sl_array_len, e_ptr);
                }
                break;

            default:
                throw sl_invalid_type("GLuint", sl_type);
        }
    }
};

template <>
struct sl_uniform_applier<GLint> {
    typedef GLint value_type;

    static void apply(const GLint* e_ptr, GLint sl_loc, GLenum sl_type, GLsizei sl_array_len, size_t sl_count) {
        if (is_float_matrix_type(sl_type) || is_double_matrix_type(sl_type)) {
            throw sl_invalid_type("GLint", sl_type);
        }
        switch (sl_count) {
            case 1:
                if (sl_array_len == 1) {
                    glUniform1i(sl_loc, e_ptr[0]);
                } else {
                    glUniform1iv(sl_loc, sl_array_len, e_ptr);
                }
                break;

            case 2:
                if (sl_array_len == 1) {
                    glUniform2i(sl_loc, e_ptr[0], e_ptr[1]);
                } else {
                    glUniform2iv(sl_loc, sl_array_len, e_ptr);
                }
                break;

            case 3:
                if (sl_array_len == 1) {
                    glUniform3i(sl_loc, e_ptr[0], e_ptr[1], e_ptr[2]);
                } else {
                    glUniform3iv(sl_loc, sl_array_len, e_ptr);
                }
                break;

            case 4:
                if (sl_array_len == 1) {
                    glUniform4i(sl_loc, e_ptr[0], e_ptr[1], e_ptr[2], e_ptr[3]);
                } else {
                    glUniform4iv(sl_loc, sl_array_len, e_ptr);
                }
                break;

            default:
                throw sl_invalid_type("GLint", sl_type);
        }
    }
};

template <>
struct sl_uniform_applier<GLfloat> {
    typedef GLfloat value_type;

    static void apply(const GLfloat* e_ptr, GLint sl_loc, GLenum sl_type, GLsizei sl_array_len, size_t sl_count) {
        if (is_double_matrix_type(sl_type)) {
            throw sl_invalid_type("GLfloat", sl_type);
        }
        switch (sl_type) {
            case GL_FLOAT_MAT2:
                glUniformMatrix2fv(sl_loc, sl_array_len, GL_FALSE, e_ptr);
                return;
            case GL_FLOAT_MAT3:
                glUniformMatrix3fv(sl_loc, sl_array_len, GL_FALSE, e_ptr);
                return;
            case GL_FLOAT_MAT4:
                glUniformMatrix4fv(sl_loc, sl_array_len, GL_FALSE, e_ptr);
                return;
            case GL_FLOAT_MAT2x3:
                glUniformMatrix2x3fv(sl_loc, sl_array_len, GL_FALSE, e_ptr);
                return;
            case GL_FLOAT_MAT2x4:
                glUniformMatrix2x4fv(sl_loc, sl_array_len, GL_FALSE, e_ptr);
                return;
            case GL_FLOAT_MAT3x2:
                glUniformMatrix3x2fv(sl_loc, sl_array_len, GL_FALSE, e_ptr);
                return;
            case GL_FLOAT_MAT3x4:
                glUniformMatrix3x4fv(sl_loc, sl_array_len, GL_FALSE, e_ptr);
                return;
            case GL_FLOAT_MAT4x2:
                glUniformMatrix4x2fv(sl_loc, sl_array_len, GL_FALSE, e_ptr);
                return;
            case GL_FLOAT_MAT4x3:
                glUniformMatrix4x3fv(sl_loc, sl_array_len, GL_FALSE, e_ptr);
                return;
        }
        switch (sl_count) {
            case 1:
                if (sl_array_len == 1) {
                    glUniform1f(sl_loc, e_ptr[0]);
                } else {
                    glUniform1fv(sl_loc, sl_array_len, e_ptr);
                }
                break;

            case 2:
                if (sl_array_len == 1) {
                    glUniform2f(sl_loc, e_ptr[0], e_ptr[1]);
                } else {
                    glUniform2fv(sl_loc, sl_array_len, e_ptr);
                }
                break;

            case 3:
                if (sl_array_len == 1) {
                    glUniform3f(sl_loc, e_ptr[0], e_ptr[1], e_ptr[2]);
                } else {
                    glUniform3fv(sl_loc, sl_array_len, e_ptr);
                }
                break;

            case 4:
                if (sl_array_len == 1) {
                    glUniform4f(sl_loc, e_ptr[0], e_ptr[1], e_ptr[2], e_ptr[3]);
                } else {
                    glUniform4fv(sl_loc, sl_array_len, e_ptr);
                }
                break;

            default:
                throw sl_invalid_type("GLfloat", sl_type);
        }
    }
};

template <>
struct sl_uniform_applier<GLdouble> {
    typedef GLdouble value_type;

    static void apply(const GLdouble* e_ptr, GLint sl_loc, GLenum sl_type, GLsizei sl_array_len, size_t sl_count) {
        if (is_float_matrix_type(sl_type)) {
            throw sl_invalid_type("GLdouble", sl_type);
        }
        switch (sl_type) {
            case GL_FLOAT_MAT2:
                glUniformMatrix2dv(sl_loc, sl_array_len, GL_FALSE, e_ptr);
                return;
            case GL_FLOAT_MAT3:
                glUniformMatrix3dv(sl_loc, sl_array_len, GL_FALSE, e_ptr);
                return;
            case GL_FLOAT_MAT4:
                glUniformMatrix4dv(sl_loc, sl_array_len, GL_FALSE, e_ptr);
                return;
            case GL_FLOAT_MAT2x3:
                glUniformMatrix2x3dv(sl_loc, sl_array_len, GL_FALSE, e_ptr);
                return;
            case GL_FLOAT_MAT2x4:
                glUniformMatrix2x4dv(sl_loc, sl_array_len, GL_FALSE, e_ptr);
                return;
            case GL_FLOAT_MAT3x2:
                glUniformMatrix3x2dv(sl_loc, sl_array_len, GL_FALSE, e_ptr);
                return;
            case GL_FLOAT_MAT3x4:
                glUniformMatrix3x4dv(sl_loc, sl_array_len, GL_FALSE, e_ptr);
                return;
            case GL_FLOAT_MAT4x2:
                glUniformMatrix4x2dv(sl_loc, sl_array_len, GL_FALSE, e_ptr);
                return;
            case GL_FLOAT_MAT4x3:
                glUniformMatrix4x3dv(sl_loc, sl_array_len, GL_FALSE, e_ptr);
                return;
        }
        switch (sl_count) {
            case 1:
                if (sl_array_len == 1) {
                    glUniform1d(sl_loc, e_ptr[0]);
                } else {
                    glUniform1dv(sl_loc, sl_array_len, e_ptr);
                }
                break;

            case 2:
                if (sl_array_len == 1) {
                    glUniform2d(sl_loc, e_ptr[0], e_ptr[1]);
                } else {
                    glUniform2dv(sl_loc, sl_array_len, e_ptr);
                }
                break;

            case 3:
                if (sl_array_len == 1) {
                    glUniform3d(sl_loc, e_ptr[0], e_ptr[1], e_ptr[2]);
                } else {
                    glUniform3dv(sl_loc, sl_array_len, e_ptr);
                }
                break;

            case 4:
                if (sl_array_len == 1) {
                    glUniform4d(sl_loc, e_ptr[0], e_ptr[1], e_ptr[2], e_ptr[3]);
                } else {
                    glUniform4dv(sl_loc, sl_array_len, e_ptr);
                }
                break;

            default:
                throw sl_invalid_type("GLdouble", sl_type);
        }
    }
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
        return uniform(ref, std::vector<T>(value));
    }

    template <typename T>
    drawer& uniform(program::var_ref ref, const T& value) {
        // typedef plain_arithmetic_container_caster<T> caster;
        // typedef sl_uniform_applier< typename caster::type > applier;
        // applier::apply( *this, ref, caster::apply(value) );
        return *this;
    }

    /* overloads for uniform 2, 3 and 4 */
    template <typename T>
    drawer& uniform(program::var_ref ref, const T& v1, const T& v2) {
        return uniform( ref, std::array<T, 2>({ v1, v2 }) );
    }

    template <typename T>
    drawer& uniform(program::var_ref ref, const T& v1, const T& v2, const T& v3) {
        return uniform( ref, std::array<T, 3>({ v1, v2, v3 }) );
    }

    template <typename T>
    drawer& uniform(program::var_ref ref, const T& v1, const T& v2, const T& v3, const T& v4) {
        return uniform( ref, std::array<T, 4>({ v1, v2, v3 }) );
    }

    /*
    template <typename T>
    drawer& attribute(program::var_ref name, const std::initializer_list<T>& value) {
        return attr(name, std::vector<T>(value));
    }

    template <typename T>
    drawer& attribute(program::var_ref name, const T& value) {
        glVertexAttrib1d(name.attribute_location(_p), 0.0);
        return *this;
    }*/

private:
    program& _p;

};


}
