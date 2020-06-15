#include <iostream>
#include <td/engine/obj.h>
#include <td/engine/engine.h>
#include <td/gl/gl.h>

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

    }

};

void td_init() {

    td::engine* e = td::engine::get();
    e->title("td_pong");
    e->add((new pong_scene())
        ->add(new ball())
    );

    /*
    td::engine& e = td::engine::get();
    e.scene(new pong_scene());*/

}