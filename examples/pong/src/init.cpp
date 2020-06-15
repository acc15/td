#include <Eigen/Dense>

#include <iostream>
#include <td/engine/obj.h>
#include <td/engine/engine.h>
#include <td/shader/program.h>
#include <td/shader/shader.h>
#include <td/gl/bo.h>
#include <td/core/assert.h>

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

    td::vbo<> TRIANGLE_VBO = td::vbo()
            << 0.f << -1.f
            << -1.f << 1.f
            << 1.f << 1.f;

    void init() override {

        SINGLE_COLOR_PROGRAM.id();

        const size_t N_BUFS = 1;
        GLuint bufs[N_BUFS];
        glGenBuffers(N_BUFS, bufs);

        TRIANGLE_VBO.apply(bufs[0]);

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

    td::engine* e = td::engine::get();
    e->title("td_pong");
    e->add(new pong_res());
    e->add((new pong_scene())
        ->add(new ball())
    );

    /*
    td::engine& e = td::engine::get();
    e.scene(new pong_scene());*/

}