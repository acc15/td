#include <catch2/catch.hpp>

#include <td/event/listener.h>

using td::emitter;
using td::listener;
using td::render_event;
using td::process_event;
using td::listener_registry;
using td::event_type;

class test_listener: public listener {
public:
    void render(const render_event& e) {
        ++render_calls;
    }

    void process(const process_event& e) {
        ++process_calls;
    }

    size_t render_calls = 0;
    size_t process_calls = 0;

};

class test_order_listener: public listener {
public:

    explicit test_order_listener(std::vector<test_order_listener*>& order_vec): _order(order_vec) {
    }

    void render(const render_event& e) {
        _order.push_back(this);
    }

private:
    std::vector<test_order_listener*>& _order;

};

class test_combined: public emitter, public listener {
public:

    size_t render_calls = 0;

    void render(const render_event& e) {
        ++render_calls;
    }

    emitter* get_this() {
        return this;
    }

};


TEST_CASE("listener") {

    test_listener l1;
    test_listener l2;

    emitter e1;
    emitter e2;

    SECTION("before tests registry must be empty") {
        REQUIRE(listener_registry::get().empty());
    }

    SECTION("can listen for events") {

        l1.listen(&e1, &test_listener::render);
        e1.emit(render_event());
        e1.emit(process_event(0.016f));

        REQUIRE(l1.render_calls == 1);
        REQUIRE(l1.process_calls == 0);

    }

    SECTION("can listen using lambda") {

        bool render_called = false;
        l1.listen(&e1, [&render_called](const render_event& e) -> void { render_called = true; });
        e1.emit(render_event());

        REQUIRE(render_called);

    }

    SECTION("can listen with priority") {

        std::vector<test_order_listener*> order_vec;

        test_order_listener o1(order_vec), o2(order_vec), o3(order_vec);

        o1.listen(&e1, &test_order_listener::render, 1000);
        o2.listen(&e1, &test_order_listener::render);
        o3.listen(&e1, &test_order_listener::render, 2000);

        e1.emit(render_event());

        REQUIRE( order_vec == std::vector<test_order_listener*>({ &o3, &o1, &o2 }) );

    }

    SECTION("can listen two times for same event") {
        l1.listen(&e1, &test_listener::render);
        l1.listen(&e1, &test_listener::render);

        e1.emit(render_event());

        REQUIRE(l1.render_calls == 2);
    }


    SECTION("can listen from multiple emitters") {

        l1.listen(&e1, &test_listener::render);
        l1.listen(&e2, &test_listener::process);

        e1.emit(render_event());
        e2.emit(process_event(0.016f));

        REQUIRE(l1.render_calls == 1);
        REQUIRE(l1.process_calls == 1);

        REQUIRE(l1.emitters() == std::unordered_set<emitter*>({&e1, &e2}));

    }

    SECTION("can listen self") {
        test_combined t;

        t.listen(&t, &test_combined::render);
        t.emit(render_event());

        REQUIRE(t.render_calls == 1);
    }

    SECTION("must unregister listener if it was destroyed") {
        {

            listener tmp_l;
            tmp_l.listen(&e1, &test_listener::render);

        }

        REQUIRE(e1.listeners().empty());
    }

    SECTION("must unregister listener if emitter was destroyed") {

        {

            emitter e;
            l1.listen(&e, &test_listener::render);
            l2.listen(&e, &test_listener::process);

            REQUIRE( l1.emitters() == std::unordered_set<emitter*>({&e}) );
            REQUIRE( l2.emitters() == std::unordered_set<emitter*>({&e}) );

        }

        REQUIRE( l1.emitters().empty() );
        REQUIRE( l2.emitters().empty() );

    }

    SECTION("mute by emitter must correctly unlink listener") {

        l1.listen(&e1, &test_listener::render);
        l1.listen(&e2, &test_listener::render);
        l2.listen(&e1, &test_listener::render);
        l2.listen(&e2, &test_listener::render);

        REQUIRE(l1.emitters() == std::unordered_set<emitter*>({ &e1, &e2 }));
        REQUIRE(l2.emitters() == std::unordered_set<emitter*>({ &e1, &e2 }));

        l1.mute(&e1);

        REQUIRE(l1.emitters() == std::unordered_set<emitter*>({ &e2 }));
        REQUIRE(l2.emitters() == std::unordered_set<emitter*>({ &e1, &e2 }));

        e1.emit(render_event());
        e2.emit(render_event());

        REQUIRE(l1.render_calls == 1);
        REQUIRE(l2.render_calls == 2);

    }

    SECTION("listener.mute must remove all listener references") {

        l1.listen(&e1, &test_listener::render);
        l1.listen(&e1, &test_listener::process);
        l1.listen(&e2, &test_listener::process);
        l2.listen(&e1, &test_listener::render);

        l1.mute();

        REQUIRE(l1.emitters().empty());

        l2.mute();

        REQUIRE(l2.emitters().empty());
        REQUIRE(listener_registry::get().empty());

    }

    SECTION("listeners by event_type correctly returned") {

        l1.listen(&e1, &test_listener::render);
        l1.listen(&e2, &test_listener::process);

        REQUIRE( e1.listeners(event_type::RENDER) == std::unordered_set<listener*>({ &l1 }) );
        REQUIRE( e2.listeners(event_type::RENDER).empty() );

        REQUIRE( e1.listeners(event_type::PROCESS).empty() );
        REQUIRE( e2.listeners(event_type::PROCESS) == std::unordered_set<listener*>({ &l1 }) );

    }

    SECTION("must correctly mute listener by tag") {

        const auto t1 = l1.listen(&e1, &test_listener::render);
        l1.listen(&e2, &test_listener::process);

        l1.mute(t1);

        e1.emit(render_event());
        e2.emit(process_event(1.f));

        REQUIRE( l1.render_calls == 0 );
        REQUIRE( l1.process_calls == 1 );

        REQUIRE( l1.emitters() == std::unordered_set<emitter*>({ &e2 }));

    }

    SECTION("activation must be removed if emitter is destroyed") {

        emitter* e_ptr = nullptr;

        {
            emitter e_tmp;
            e_ptr = &e_tmp;
            e_tmp.activation(event_type::PROCESS, [](bool active) -> void {});
            REQUIRE(listener_registry::get().has_activations(e_ptr));
        }

        REQUIRE_FALSE(listener_registry::get().has_activations(e_ptr));
        REQUIRE(listener_registry::get().empty());

    }

    SECTION("activation function called correctly") {

        std::vector<bool> activation_calls;

        emitter e;
        e.activation(event_type::PROCESS, [&activation_calls](bool active) -> void {
            activation_calls.emplace_back(active);
        });

        l1.listen(&e, &test_listener::process);
        REQUIRE( activation_calls == std::vector<bool>({ true }) );

        l2.listen(&e, &test_listener::process);
        REQUIRE( activation_calls == std::vector<bool>({ true }) );

        l2.mute(&e);
        REQUIRE( activation_calls == std::vector<bool>({ true }) );

        l1.mute(&e);
        REQUIRE( activation_calls == std::vector<bool>({ true, false }) );

    }

    SECTION("multi-event activation must be called correctly") {

        std::vector<bool> activation_calls;

        emitter e;
        e.activation({ event_type::PROCESS, event_type::RENDER }, [&activation_calls](bool active) -> void {
            activation_calls.emplace_back(active);
        });

        auto t1 = l1.listen(&e, &test_listener::process);
        REQUIRE(activation_calls == std::vector<bool>({ true }));

        auto t2 = l1.listen(&e, &test_listener::render);
        REQUIRE(activation_calls == std::vector<bool>({ true }));

        l1.mute(t1);
        REQUIRE(activation_calls == std::vector<bool>({ true }));

        l1.mute(t2);
        REQUIRE(activation_calls == std::vector<bool>({ true, false }));

    }


    SECTION("randomized test") {

        const size_t N = 100;

        std::random_device dev;
        std::default_random_engine generator(dev());
        std::uniform_int_distribution<size_t> distribution(0, N - 1);

        emitter es[N];
        test_listener ls[N];

        std::vector<size_t> expected_counts(N, 0);
        for (auto& e : es) {

            size_t index = distribution(generator);
            ++expected_counts[index];

            listener* l = &ls[index];
            l->listen(&e, &test_listener::render);

        }

        for (size_t i = 0; i < N; i++) {
            INFO(i << ": " << std::string(ls[i].emitters().size(), '#'));
        }

        for (auto& e : es) {
            e.emit(render_event());
        }

        std::vector<size_t> actual_counts;
        for (auto & l : ls) {
            l.mute();
            actual_counts.push_back(l.render_calls);
        }

        REQUIRE(expected_counts == actual_counts);
        REQUIRE(listener_registry::get().empty());

    }

    SECTION("after tests registry must be empty") {
        REQUIRE(listener_registry::get().empty());
    }


}