#include <iostream>
#include <td/engine/obj.h>
#include <td/engine/engine.h>

class pong_scene: public td::obj {
public:
    void attach() override {
        listen(td::engine::get(), &pong_scene::key_down);
        forward<td::render_event>(td::engine::get());
    }

    void key_down(const td::key_down_event& e) {
        std::cout << "key pressed " << e.scan_code() << std::endl;
    }

};

class ball: public td::obj {
public:
    void attach() override {
        listen(td::engine::get(), &ball::render);
    }

    void render() {

    }

};

template<typename T, typename ... Args>
T& mk(Args... args) {
    return *new T(args...);
}

void td_init() {

    td::engine& e = td::engine::get();
    e.title("td_pong");

    e.add(mk<pong_scene>()
        .add(mk<ball>())
    );

    /*
    td::engine& e = td::engine::get();
    e.scene(new pong_scene());*/

}