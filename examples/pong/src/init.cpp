#include <iostream>
#include <td/engine/obj.h>
#include <td/engine/engine.h>
#include <td/gl/program.h>
#include <td/gl/bo.h>
#include <td/gl/vbo_layout.h>
#include <td/gl/enum.h>
#include <td/gl/drawer.h>

class pong_res: public td::obj {
public:
    static constexpr char TAG[] = "res";

    td::shader POSITION_MVP_VERTEX = td::shader(GL_VERTEX_SHADER, R"#(
#version 330

uniform mat3 u_mvpMatrix;
in vec2 a_Position;

void main()
{
    gl_Position = vec4(u_mvpMatrix * vec3(a_Position, 0), 1);
}
        )#");

    td::shader SINGLE_COLOR_FRAGMENT = td::shader(GL_FRAGMENT_SHADER, R"#(
#version 330

uniform vec4 u_Color;
out vec4 out_Color;

void main() {
    out_Color = u_Color;
}
        )#");

    td::program SINGLE_COLOR_PROGRAM = td::program()
        .add(POSITION_MVP_VERTEX)
        .add(SINGLE_COLOR_FRAGMENT);

    td::vbo<> TRIANGLE_VBO = td::vbo<>()
            << 0.f << -1.f
            << -1.f << 1.f
            << 1.f << 1.f;

    td::vbo_layout TRIANGLE_VBO_LAYOUT = td::vbo_layout().f2("a_Position");

    void init() override {

        tag(TAG);

        SINGLE_COLOR_PROGRAM.link();

        const size_t N_BUFS = 1;
        GLuint bufs[N_BUFS];
        glGenBuffers(N_BUFS, bufs);

        TRIANGLE_VBO.apply(bufs[0]);

    }

};


class pong_scene: public td::obj {
public:
    void init() override {
        listen(td::engine::get(), &pong_scene::key_down);
        forward<td::render_event>(td::engine::get());
        forward<td::process_event>(td::engine::get());
    }

    void key_down(const td::key_down_event& e) {
        std::cout << "key pressed " << e.scan_code() << std::endl;
    }

};

class ball: public td::obj {
public:
    float _scale = 1.f;
    float _dir = -0.02f;

    void init() override {
        listen(parent<pong_scene>(), &ball::process);
        listen(parent<pong_scene>(), &ball::render);
    }

    void process(const td::process_event& e) {
        _scale += _dir;
        if (_scale < 0.5f) {
            _scale = 0.5f;
            _dir = -_dir;
        } else if (_scale > 1.f) {
            _scale = 1.f;
            _dir = -_dir;
        }
    }

    void render(const td::render_event& e) {

        auto* res = by_tag<pong_res>();
        td::drawer(res->SINGLE_COLOR_PROGRAM)
                .uniform("u_mvpMatrix", { _scale, 0.f, 0.f, 0.f, _scale, 0.f, 0.f, 0.f, 1.f })
                .uniform("u_Color", { 1.f, 0.f, 0.f, 1.f })
                // .attribute("a_Position", res->TRIANGLE_VBO, 0, GL_FLOAT, 2, 2 * sizeof(GLfloat))
                .attribute("a_Position", res->TRIANGLE_VBO, res->TRIANGLE_VBO_LAYOUT)
                .draw(GL_TRIANGLES);

    }

};

void td_init() {

    td::engine* e = td::engine::get();
    e->title("td_pong");
    e->add(new pong_res());
    e->add((new pong_scene())
        ->add(new ball())
    );

}