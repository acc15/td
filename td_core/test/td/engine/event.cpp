#include <catch2/catch.hpp>
#include <td/engine/event.h>
#include <td/engine/object.h>

class test_handler: public td::engine_object {
public:
    size_t render_called[2] = { 0, 0 };

    void init() override {
    }

    void render(const td::render_event& e) {
        ++render_called[0];
    }

    void render2(const td::render_event& e2) {
        ++render_called[1];
    }

};

TEST_CASE("listener_list") {
    test_handler handler;
    td::listener_list l;

    SECTION("listen and mute by tag") {
        l.listen(&handler, td::eh(&test_handler::render), 0);
        auto l2 = l.listen(&handler, td::eh(&test_handler::render2), 0);

        l.emit(td::render_event());
        l.mute(l2);
        l.emit(td::render_event());

        REQUIRE(handler.render_called[0] == 2);
        REQUIRE(handler.render_called[1] == 1);
    }

    SECTION("mute by owner") {

        test_handler handler2;

        l.listen(&handler2, td::eh(&test_handler::render2), 0);
        l.listen(&handler, td::eh(&test_handler::render), 0);
        l.listen(&handler, td::eh(&test_handler::render), 0);
        l.listen(&handler2, td::eh(&test_handler::render), 0);
        l.listen(&handler, td::eh(&test_handler::render2), 0);

        l.emit(td::render_event());

        l.mute(&handler);

        l.emit(td::render_event());

        REQUIRE(handler2.render_called[0] == 2);
        REQUIRE(handler2.render_called[1] == 2);
        REQUIRE(handler.render_called[0] == 2);
        REQUIRE(handler.render_called[1] == 1);

    }

}

TEST_CASE("event_emitter") {
    test_handler handler;

    td::event_emitter e;
    e.listen(td::RENDER, &handler, reinterpret_cast<td::event_handler>(&test_handler::render));

    td::render_event re;
    e.emit(re);

    REQUIRE( handler.render_called );
}