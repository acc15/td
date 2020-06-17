#include <string>
#include <stdexcept>

#include <fmt/format.h>

#include <td/gl/program.h>

namespace td {

program::program() : _id(0) {
}


program::program(program&& mv) noexcept:
        _id(mv._id),
        _uniform_locs(std::move(mv._uniform_locs)),
        _attr_locs(std::move(mv._attr_locs)),
        _uniforms(std::move(mv._uniforms)),
        _attrs(std::move(mv._attrs)),
        _uniform_map(std::move(mv._uniform_map)),
        _attr_map(std::move(mv._attr_map)),
        _bind_attrs(mv._bind_attrs),
        _internals(std::move(mv._internals)),
        _externals(std::move(mv._externals))
{
    mv._id = 0;
}

program::~program() {
    rm();
}

program& program::operator=(program&& mv) noexcept {
    rm();
    std::swap(_id, mv._id);
    _bind_attrs = mv._bind_attrs;
    _internals = std::move(mv._internals);
    _externals = std::move(mv._externals);
    _uniform_locs = std::move(mv._uniform_locs);
    _attr_locs = std::move(mv._attr_locs);
    _uniforms = std::move(mv._uniforms);
    _attrs = std::move(mv._attrs);
    _attr_map = std::move(mv._attr_map);
    _uniform_map = std::move(mv._uniform_map);
    return *this;
}


program&& program::add(shader&& mv) {
    _internals.push_back(mv);
    return this_mv();
}

program&& program::add(shader& cp) {
    _externals.push_back(std::ref(cp));
    return this_mv();
}

program&& program::add(shader_type type, const char* src) {
    return add(shader(type, src));
}

program&& program::vertex(const char* src) {
    return add(shader_type::VERTEX, src);
}

program&& program::fragment(const char* src) {
    return add(shader_type::FRAGMENT, src);
}

program&& program::bind_attr(const char* name, GLuint index) {
    _bind_attrs[name] = index;
    return this_mv();
}

const std::vector<std::reference_wrapper<shader>>& program::externals() const {
    return _externals;
}

const std::vector<shader>& program::internals() const {
    return _internals;
}


GLuint program::id() {
    if (_id != 0) {
        return _id;
    }

    for (td::shader& s: _internals) s.id();
    for (td::shader& s: _externals) s.id();

    GLuint id = glCreateProgram();
    if (id == 0) {
        throw std::runtime_error("Unable to create GL program object");
    }

    for (td::shader& s: _internals) glAttachShader(id, s.id());
    for (td::shader& s: _externals) glAttachShader(id, s.id());

    for (const auto& l: _bind_attrs) {
        glBindAttribLocation(id, l.second, l.first);
    }

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

        throw std::runtime_error(fmt::format("Unable to link program: {}", info_log));
    }

    populate_uniforms(id);
    populate_attributes(id);

    _id = id;
    return _id;
}


GLint program::uniform_location(const char* name) const {
    auto iter = _uniform_locs.find(name);
    if (iter == _uniform_locs.end()) {
        throw std::invalid_argument(fmt::format("unknown uniform name: {}", name));
    }
    return iter->second;
}

GLuint program::attribute_location(const char* name) const {
    auto iter = _attr_locs.find(name);
    if (iter == _attr_locs.end()) {
        throw std::invalid_argument(fmt::format("unknown attribute name: {}", name));
    }
    return iter->second;
}

const sl_var& program::uniform(sl_var_ref ref) const {
    GLint loc = ref.uniform_location(*this);
    auto iter = _uniform_map.find(loc);
    if (iter == _uniform_map.end()) {
        throw std::invalid_argument(fmt::format("unknown uniform location: {}", loc));
    }
    return *iter->second;
}

const sl_var& program::attribute(sl_var_ref ref) const {
    GLint loc = ref.attribute_location(*this);
    auto iter = _attr_map.find(loc);
    if (iter == _attr_map.end()) {
        throw std::invalid_argument(fmt::format("unknown attribute location: {}", loc));
    }
    return *iter->second;
}

const std::vector<sl_var>& program::uniforms() const {
    return _uniforms;
}

const std::vector<sl_var>& program::attributes() const {
    return _attrs;
}

void program::rm() {
    if (_id == 0) {
        return;
    }

    glDeleteProgram(_id);
    _id = 0;

    _uniform_map.clear();
    _attr_map.clear();
    _uniforms.clear();
    _attrs.clear();
    _uniform_locs.clear();
    _attr_locs.clear();

}

program&& program::this_mv() {
    return std::move( *this );
}

void program::populate_uniforms(GLuint id) {
    GLint uniform_count, uniform_max_length;
    glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &uniform_count);
    glGetProgramiv(id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniform_max_length);

    for (GLuint i = 0; i < static_cast<GLuint>(uniform_count); i++) {

        std::string name(uniform_max_length, '\0');
        GLsizei uniform_length;
        GLint size;
        GLenum type;

        glGetActiveUniform(id, i, uniform_max_length, &uniform_length, &size, &type, name.data());
        name.resize(uniform_length);

        GLint location = glGetUniformLocation(id, name.data());

        auto loc_iter = _uniform_locs.insert(std::make_pair(std::move(name), location)).first;
        _uniforms.push_back({ location, i, type, size, loc_iter->first });
        _uniform_map[location] = &_uniforms.back();
    }
}

void program::populate_attributes(GLuint id) {
    GLint attribute_count, attribute_max_length;
    glGetProgramiv(id, GL_ACTIVE_ATTRIBUTES, &attribute_count);
    glGetProgramiv(id, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &attribute_max_length);
    for (GLuint i = 0; i < static_cast<size_t>(attribute_count); i++) {

        std::string name(attribute_max_length, '\0');
        GLsizei attr_length;
        GLint size;
        GLenum type;

        glGetActiveAttrib(id, i, attribute_max_length, &attr_length, &size, &type, name.data());
        name.resize(attr_length);

        GLint location = glGetAttribLocation(id, name.data());

        auto loc_iter = _attr_locs.insert(std::make_pair(std::move(name), location)).first;
        _attrs.push_back({ location, i, type, size, loc_iter->first });
        _attr_map[location] = &_attrs.back();

    }
}



}
