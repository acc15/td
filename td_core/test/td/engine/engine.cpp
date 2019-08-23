#include <catch2/catch.hpp>
#include <td/engine/event.h>
#include <td/engine/object.h>

class test_handler: public td::engine_object {
public:
    size_t render_called[2] = { 0, 0 };

    void on_attach() override {
        //engine()->listen(td::RENDER, )
    }

    void render(const td::render_event& e) {
        ++render_called[0];
    }
};

TEST_CASE("engine") {

    SECTION("events") {

        test_handler handler;

        td::engine e;
        e.listen(td::RENDER, &handler, reinterpret_cast<td::event_handler>(&test_handler::render));

        td::render_event re;
        e.emit(re);

        REQUIRE( handler.render_called );
    }

}