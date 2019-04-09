#include <td/core/program.h>

program::program(): _id(0) {
}

program::program(program&& mv) noexcept: _id(mv._id), _internals(std::move(mv._internals)), _externals(std::move(mv._externals)) {
    mv._id = 0;
}

program& program::operator=(program &&mv) noexcept {
    rm();
    std::swap(_id, mv._id);
    _internals = std::move(mv._internals);
    _externals = std::move(mv._externals);
    return *this;
}

program&& program::add(td::shader&& mv) {
    _internals.push_back(mv);
    return std::move(*this);
}

program&& program::add(td::shader& cp) {
    _externals.push_back(std::ref(cp));
    return std::move(*this);
}

GLuint program::id() {
    if (_id != 0) {
        return _id;
    }

    for (td::shader& s: _internals) s.id();
    for (td::shader& s: _externals) s.id();

    GLuint id = glCreateProgram();
    if (id == 0) {
        // todo improve error message
        throw std::runtime_error("Unable to create program");
    }

    for (td::shader& s: _internals) glAttachShader(id, s.id());
    for (td::shader& s: _externals) glAttachShader(id, s.id());

    glLinkProgram(id);

    for (td::shader& s: _internals) glDetachShader(id, s.id());
    for (td::shader& s: _externals) glDetachShader(id, s.id());

    for (td::shader& s: _internals) s.rm();

    GLint link_status = 0;
    glGetProgramiv(id, GL_LINK_STATUS, &link_status);
    if (link_status == GL_FALSE) {

        GLint log_length = 0;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &log_length);

        std::string info_log(log_length, '\0');
        glGetProgramInfoLog(id, log_length, nullptr, info_log.data());

        glDeleteProgram(id);

        throw std::runtime_error("Unable to link program: " + info_log);
    }

    _id = id;
    return _id;
}

void program::rm() {
    if (_id == 0) {
        return;
    }

    glDeleteProgram(_id);
    _id = 0;
}
