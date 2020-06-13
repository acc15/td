#include <iostream>
#include <td/engine/engine.h>
#include <td/event/listener.h>

class pong_scene: public td::listener {
public:
    pong_scene() {
        listen(td::engine::get(), &pong_scene::key_down);
    }

    void key_down(const td::key_down_event& e) {
        std::cout << "key pressed " << e.scan_code() << std::endl;
    }

};


void td_init() {

    td::engine& e = td::engine::get();
    e.title("td_pong");

    /*
    td::engine& e = td::engine::get();
    e.scene(new pong_scene());*/

}