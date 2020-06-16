#include <Eigen/Dense>

#include <iostream>
#include <td/engine/obj.h>
#include <td/engine/engine.h>
#include <td/gl/program.h>
#include <td/gl/shader.h>
#include <td/gl/bo.h>
#include <td/gl/enum.h>


#include <numeric>
#include <stdexcept>
#include <td/gl/shader_var_info.h>

namespace td {

struct uniform_resolver {
    inline static GLuint resolve(GLuint p, const char* name) {
        return glGetUniformLocation(p, name);
    }
};

struct attr_resolver {
    inline static GLuint resolve(GLuint p, const char* name) {
        return glGetAttribLocation(p, name);
    }
};

template <typename Resolver>
class program_ref {
public:

    program_ref(GLint location): _name(nullptr), _location(location) {
    }

    program_ref(const char* name): _name(name), _location(-1) {
    }

    GLint location(GLuint p) {
        if (_location != -1) {
            return _location;
        }
        if (_name != nullptr) {
            _location = Resolver::resolve(p, _name);
        }
        if (_location == -1) {
            throw std::invalid_argument(_name != nullptr
                ? fmt::format("Invalid uniform name: {}, location: {}", _name, _location)
                : fmt::format("Invalid uniform location {}", _location));
        }
        return _location;
    }

private:

    const char* _name;
    GLint _location;

};

typedef program_ref<uniform_resolver> uniform_ref;
typedef program_ref<attr_resolver> attribute_ref;


class drawer {
public:

    // only external references accepted
    // this is intentional to avoid compiling program every draw operation
    drawer(td::program& p): _p(p.id()) {
        glUseProgram(_p);
    }

    template <typename T>
    drawer& uniform(uniform_ref name, const std::initializer_list<T>& value) {
        return uniform(name, std::vector<T>(value));
    }

    template <typename T>
    drawer& uniform(uniform_ref name, const T& value) {
        glUniform2d(name.location(_p), 0.0, 0.0);
        return *this;
    }

    template <typename T>
    drawer& attr(attribute_ref name, const std::initializer_list<T>& value) {
        return attr(name, std::vector<T>(value));
    }

    template <typename T>
    drawer& attr(attribute_ref name, const T& value) {
        glVertexAttrib1d(name.location(_p), 0.0);
        return *this;
    }

private:
    GLuint _p;

};

}

class pong_res: public td::obj {
public:
    static constexpr char TAG[] = "res";

    td::shader POSITION_MVP_VERTEX = td::shader(td::shader_type::VERTEX, R"#(
        in vec4 a_Position;
        uniform mat4 u_mvpMatrix;
        void main()
        {
            gl_Position = u_mvpMatrix * a_Position;
        })#");

    td::shader SINGLE_COLOR_FRAGMENT = td::shader(td::shader_type::FRAGMENT, R"#(
        uniform vec4 u_Color;
        void main() {
            gl_FragColor = u_Color;
        })#");

    td::program SINGLE_COLOR_PROGRAM = td::program()
        .add(POSITION_MVP_VERTEX)
        .add(SINGLE_COLOR_FRAGMENT);

    td::vbo<> TRIANGLE_VBO = td::vbo<>()
            << 0.f << -1.f
            << -1.f << 1.f
            << 1.f << 1.f;

    void init() override {

        SINGLE_COLOR_PROGRAM.id();

        const size_t N_BUFS = 1;
        GLuint bufs[N_BUFS];
        glGenBuffers(N_BUFS, bufs);

        TRIANGLE_VBO.apply(bufs[0]);

        Eigen::Vector4f m;
        std::cout << m.rows() << " " << m.cols() << std::endl;

        /*
        td::drawer(SINGLE_COLOR_PROGRAM)
            .uniform("u_mvpMatrix", { 1.f, 2.f, 3.f })
            .attr(1, { 1.f });*/


        // TRIANGLE_VBO.apply(); - calls glGenBuffers(1, &n);
        // TRIANGLE_VBO.apply(bufs[0]);, GL_STATIC_DRAW); - remembers id

        // TRIANGLE_VBO.reset() <<
        // TRIANGLE_VBO.apply();, GL_STATIC_DRAW); - remembers id
            //glBindBuffer(GL_ARRAY_BUFFER, id);
            //glBufferData(GL_ARRAY_BUFFER, size(), data(), usage);

    }

};


class pong_scene: public td::obj {
public:
    void init() override {
        listen(td::engine::get(), &pong_scene::key_down);
        forward<td::render_event>(td::engine::get());
    }

    void key_down(const td::key_down_event& e) {
        std::cout << "key pressed " << e.scan_code() << std::endl;
    }

};

class ball: public td::obj {
public:
    void init() override {
        listen(parent<pong_scene>(), &ball::render);
    }

    void render(const td::render_event& e) {
        // glDrawElements()
    }

};

void td_init() {


    td::program test_program = td::program()
            .add(td::shader(td::shader_type::VERTEX, R"#(
#version 330

in vec2 b_Position;
in vec2 a_Position;
uniform mat3 u_mvpMatrix;

void main() {
    gl_Position = vec4(u_mvpMatrix * vec3(a_Position, 1.0) + vec3(b_Position, 1.0), 1.0);
}
            )#"))
            .add(td::shader(td::shader_type::FRAGMENT, R"#(
#version 330

uniform vec4 u_Color;
out vec4 color;

void main() {
    color = u_Color;
}
            )#"));

    GLuint p = test_program.id();

    std::vector<td::shader_var_info> uniforms = td::get_active_uniforms(p);
    std::vector<td::shader_var_info> attrs = td::get_active_attrs(p);

    GLuint bb;
    glGenBuffers(-1, &bb);

    std::cout << "u_mvpMatrix location: " << glGetUniformLocation(p, "u_mvpMatrix") << std::endl;
    std::cout << "u_Color location: " << glGetUniformLocation(p, "u_Color") << std::endl;
    std::cout << "a_Position location: " << glGetAttribLocation(p, "a_Position") << std::endl;
    std::cout << "b_Position location: " << glGetAttribLocation(p, "b_Position") << std::endl;

    std::cout << "Uniform count: " << uniforms.size() << std::endl;
    for (size_t i = 0; i < uniforms.size(); i++) {
        std::cout
            << "Uniform [#" << i << "] name \"" << uniforms[i].name
            << "\" type " << td::gl_enum_string(uniforms[i].type)
            << " size " << uniforms[i].size
            << std::endl;
    }

    std::cout << "Attribute count: " << attrs.size() << std::endl;
    for (size_t i = 0; i < attrs.size(); i++) {
        std::cout
            << "Attribute [#" << i << "] name \"" << attrs[i].name
            << "\" type " << td::gl_enum_string(attrs[i].type)
            << " size " << attrs[i].size
            << std::endl;
    }
    /*
    td::engine* e = td::engine::get();
    e->title("td_pong");
    e->add(new pong_res());
    e->add((new pong_scene())
        ->add(new ball())
    );*/

    /*
    td::engine& e = td::engine::get();
    e.scene(new pong_scene());*/

}