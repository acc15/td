#include <catch2/catch.hpp>
#include <td/engine/event.h>
#include <td/engine/object.h>

class test_handler: public td::engine_object {
public:
    bool render_called = false;

    void init() override {
    }

    void render(const td::render_event& e) {
        render_called = true;
    }
};


TEST_CASE("listen", "[event_emitter]" ) {
    test_handler handler;

    td::event_emitter e;
    e.listen(td::RENDER, &handler, reinterpret_cast<td::event_handler>(&test_handler::render));

    td::render_event re;
    e.emit(re);

    REQUIRE( handler.render_called );
}