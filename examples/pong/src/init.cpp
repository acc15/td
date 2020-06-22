#define _USE_MATH_DEFINES // for C++
#include <cmath>

#include <iostream>

#include <Eigen/Geometry>

#include <td/engine/obj.h>
#include <td/engine/engine.h>
#include <td/gl/program.h>
#include <td/gl/bo.h>
#include <td/gl/vbo_layout.h>
#include <td/gl/drawer.h>


static constexpr float TWO_M_PI = 2.f * static_cast<float>(M_PI);

class pong_res: public td::obj {
public:
    static constexpr char TAG[] = "res";

    td::shader POSITION_MVP_VERTEX = td::shader(GL_VERTEX_SHADER, R"#(
#version 330

uniform mat3 u_mvpMatrix;
in vec2 a_Position;
in vec4 a_Color;

out vec4 f_Color;

void main()
{
    f_Color = a_Color;
    gl_Position = vec4(u_mvpMatrix * vec3(a_Position, 0), 1);
}
        )#");

    td::shader SINGLE_COLOR_FRAGMENT = td::shader(GL_FRAGMENT_SHADER, R"#(
#version 330

in vec4 f_Color;
out vec4 out_Color;

void main() {
    out_Color = f_Color;
}
        )#");

    td::program SINGLE_COLOR_PROGRAM = td::program()
        .add(POSITION_MVP_VERTEX)
        .add(SINGLE_COLOR_FRAGMENT);

    static constexpr float TRIANGLE_R = 200.f;

    td::vbo<> TRIANGLE_VBO = td::vbo<>()
            << cosf(TWO_M_PI / 3.f * 0.f) * TRIANGLE_R << sinf(TWO_M_PI / 3.f * 0.f) * TRIANGLE_R << 1.f << 0.f << 0.f << 1.f
            << cosf(TWO_M_PI / 3.f * 1.f) * TRIANGLE_R << sinf(TWO_M_PI / 3.f * 1.f) * TRIANGLE_R << 0.f << 1.f << 0.f << 1.f
            << cosf(TWO_M_PI / 3.f * 2.f) * TRIANGLE_R << sinf(TWO_M_PI / 3.f * 2.f) * TRIANGLE_R << 0.f << 0.f << 1.f << 1.f;

    td::vbo_layout TRIANGLE_VBO_LAYOUT = td::vbo_layout().f2("a_Position").f4("a_Color");

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

    static constexpr char TAG[] = "scene";

    Eigen::Affine2f _world;

    void init() override {
        tag(TAG);

        listen(parent<td::emitter>(), &pong_scene::key_down);
        listen(parent<td::emitter>(), &pong_scene::resize);

        forward<td::render_event>(td::engine::get());
        forward<td::process_event>(td::engine::get());
    }

    void resize(const td::resize_event& e) {
        const Eigen::Vector2f vp = td::engine::get()->viewport();
        _world.setIdentity();
        _world = _world.scale(vp / 2).inverse();
    }

    void key_down(const td::key_down_event& e) {
        std::cout << "key pressed " << e.scan_code << std::endl;
    }

};

class ball: public td::obj {
public:
    static constexpr float MIN_SCALE = 0.25f;
    static constexpr float MAX_SCALE = 1.f;

    float _scale = 1.f;
    float _rot = 0.f;
    float _dir = -0.5f;
    float _rot_speed = 1;


    Eigen::Affine2f _t;

    void init() override {
        listen(parent<pong_scene>(), &ball::process);
        listen(parent<pong_scene>(), &ball::render);
    }

    void process(const td::process_event& e) {
        _scale += _dir * e.duration;
        if (_scale < MIN_SCALE) {
            _scale = MIN_SCALE;
            _dir = -_dir;
        } else if (_scale > MAX_SCALE) {
            _scale = MAX_SCALE;
            _dir = -_dir;
        }
        _rot += _rot_speed * e.duration;
        if (_rot > TWO_M_PI) {
            _rot -= TWO_M_PI;
        }

        _t.setIdentity();
        _t.scale(_scale).rotate(_rot);

        _t = by_tag<pong_scene>()->_world * _t;

    }

    void render(const td::render_event& e) {
        auto* res = by_tag<pong_res>();
        td::drawer(res->SINGLE_COLOR_PROGRAM)
                .uniform("u_mvpMatrix", _t.matrix())
                .attribute("a_Position", res->TRIANGLE_VBO, res->TRIANGLE_VBO_LAYOUT)
                .attribute("a_Color", res->TRIANGLE_VBO, res->TRIANGLE_VBO_LAYOUT)
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