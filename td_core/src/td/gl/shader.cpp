#include <stdexcept>
#include <fmt/format.h>
#include <td/gl/shader.h>

namespace td {

shader::shader(const shader_type& type, const char *src): _id(0), _src(src), _type(type) {
}

shader::~shader() {
    rm();
}

GLuint shader::compile() {
    if (_id != 0) {
        return _id;
    }

    GLuint id = glCreateShader(_type.value());
    if (id == 0) {
        throw std::runtime_error(fmt::format("Unable to create shader of type {}. OpenGL error: {}",
            _type.name(), glGetError())
        );
    }
    glShaderSource(id, 1, &_src, nullptr);
    glCompileShader(id);

    GLint success = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (success != GL_FALSE) {
        _id = id;
        return _id;
    }

    GLint log_size = 0;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_size);

    std::string info_log(log_size, '\0');
    glGetShaderInfoLog(id, log_size, nullptr, info_log.data());

    throw std::runtime_error("Unable to compile shader: " + info_log);
}

void shader::rm() {
    if (_id == 0) {
        return;
    }
    glDeleteShader(_id);
    _id = 0;
}

GLuint shader::id() const {
    return _id;
}

const char* shader::src() const {
    return _src;
}

const shader_type& shader::type() const {
    return _type;
}

}
