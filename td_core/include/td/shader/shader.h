#pragma once

#include <stdexcept>
#include "../gl/gl.h"

namespace td {

enum class shader_type {
    VERTEX,
    FRAGMENT
};

inline std::string shader_type_str(shader_type t) {
    switch (t) {
    case shader_type::VERTEX: return "VERTEX";
    case shader_type::FRAGMENT: return "FRAGMENT";
    default: return "UNKNOWN (numeric " + std::to_string(static_cast<int>(t));
    }
}

inline GLenum shader_type_gl(shader_type t) {
    switch (t) {
    case shader_type::VERTEX: return GL_VERTEX_SHADER;
    case shader_type::FRAGMENT: return GL_FRAGMENT_SHADER;
    default: throw std::invalid_argument("unknown shader_type " + shader_type_str(t));
    }
}


class shader {

public:

    shader(shader_type type, const char* src);
    ~shader();

    const char* src() const;
    shader_type type() const;

    void rm();
    GLuint id();

private:
    GLuint _id;

    const char* _src;
    shader_type _type;

};


}



