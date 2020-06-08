#include <catch2/catch.hpp>

#include <map>
#include <unordered_map>
#include <unordered_set>

enum class event_type {
    RENDER,
    PROCESS
};

class event {
public:
    virtual event_type type() const = 0;
};

class render_event: public event {
public:
    static const event_type TYPE = event_type::RENDER;

    event_type type() const override {
        return TYPE;
    }
};

class process_event: public event {
public:
    static const event_type TYPE = event_type::PROCESS;

    event_type type() const override {
        return TYPE;
    }

};

template <typename L, typename E>
using listener_handler = void (L::*)(const E& event);

class listener;
class emitter;

class listener_registry {
private:
    listener_registry() = default;

    typedef std::pair<listener*, listener_handler<listener, event>> listener_pair;
    typedef std::multimap<size_t, listener_pair, std::greater<> > priority_map;
    typedef std::unordered_map<event_type, priority_map> event_type_map;
    typedef std::unordered_map<emitter*, event_type_map> emitter_map;

    typedef std::unordered_multimap<event_type, priority_map::iterator> listener_iterator_map;
    typedef std::unordered_map<emitter*, listener_iterator_map> listener_emitter_map;
    typedef std::unordered_map<listener*, listener_emitter_map> listener_ref_map;

    emitter_map _e;
    listener_ref_map _l;

    template <typename Col>
    static bool erase_if_nested_empty(const typename Col::iterator& i, Col& c) {
        if (!i->second.empty()) {
            return false;
        }
        c.erase(i);
        return true;
    }

    void unlink_by_iterators(const std::pair<emitter*, const listener_iterator_map&>& le_entry) {
        const auto e_iter = _e.find(le_entry.first);
        if (e_iter == _e.end()) {
            return;
        }
        for (const auto& event_ref: le_entry.second) {
            const auto et_iter = e_iter->second.find(event_ref.first);
            if (et_iter == e_iter->second.end()) {
                continue;
            }
            et_iter->second.erase(event_ref.second);
            erase_if_nested_empty(et_iter, e_iter->second);
        }
        erase_if_nested_empty(e_iter, _e);
    }


public:
    struct listener_tag {
        emitter* e;
        listener* l;
        event_type t;
        priority_map::iterator i;
    };

    static listener_registry& instance() {
        static listener_registry i;
        return i;
    }

    template<typename L, typename E>
    listener_tag link(emitter* e, listener* l, listener_handler<L, E> h, size_t p) {
        const event_type t = E::TYPE;
        const auto eh = reinterpret_cast<listener_handler<listener, event>>(h);
        const auto iter = _e[e][t].insert(std::make_pair(p, std::make_pair(l, eh)));
        _l[l][e].insert(std::make_pair(t, iter));
        return { e, l, t, iter };
    }

    void unlink(emitter* e) {

        const auto e_iter = _e.find(e);
        if (e_iter == _e.end()) {
            return;
        }

        for (const auto& event_type_entry: e_iter->second) {
            for (const auto& priority_entry: event_type_entry.second) {
                listener* l = priority_entry.second.first;

                const auto l_iter = _l.find(l);
                if (l_iter == _l.end()) {
                    continue;
                }

                l_iter->second.erase(e);
                erase_if_nested_empty(l_iter, _l);

            }
        }
        _e.erase(e);

    }

    void unlink(listener* l) {
        const auto l_iter = _l.find(l);
        if (l_iter == _l.end()) {
            return;
        }
        for (const auto& le_entry: l_iter->second) {
            unlink_by_iterators(le_entry);
        }
        _l.erase(l);
    }


    void unlink(listener* l, emitter* e) {

        const auto l_iter = _l.find(l);
        if (l_iter == _l.end()) {
            return;
        }

        const auto le_iter = l_iter->second.find(e);
        if (le_iter == l_iter->second.end()) {
            return;
        }

        unlink_by_iterators(*le_iter);

        l_iter->second.erase(le_iter);
        erase_if_nested_empty(l_iter, _l);

    }

    void unlink(const listener_tag& t) {

        const auto e_iter = _e.find(t.e);
        if (e_iter != _e.end()) {
            const auto et_iter = e_iter->second.find(t.t);
            if (et_iter != e_iter->second.end()) {
                et_iter->second.erase(t.i);
                if (erase_if_nested_empty(et_iter, e_iter->second)) {
                    erase_if_nested_empty(e_iter, _e);
                }
            }
        }

        const auto l_iter = _l.find(t.l);
        if (l_iter != _l.end()) {
            const auto le_iter = l_iter->second.find(t.e);
            if (le_iter != l_iter->second.end()) {
                const auto let_range = le_iter->second.equal_range(t.t);
                for (auto iter = let_range.first; iter != let_range.second; ++iter) {
                    if (iter->second != t.i) {
                        continue;
                    }
                    iter = le_iter->second.erase(iter);
                    if (erase_if_nested_empty(le_iter, l_iter->second)) {
                        erase_if_nested_empty(l_iter, _l);
                    }
                    break;
                }
            }
        }

    }

    template <typename E>
    void emit(emitter* e, const E& event) {
        const auto e_iter = _e.find(e);
        if (e_iter == _e.end()) {
            return;
        }

        const event_type t = E::TYPE;
        const auto et_iter = e_iter->second.find(t);
        if (et_iter == e_iter->second.end()) {
            return;
        }
        for (const auto& p_entry: et_iter->second) {
            (p_entry.second.first->*p_entry.second.second)(event);
        }
    }

    std::unordered_set<emitter*> emitters_of(const listener* l) const {
        std::unordered_set<emitter*> res;
        const auto i = _l.find(const_cast<listener*>(l));
        if (i == _l.end()) {
            return res;
        }
        for (const auto& emitter_entry : i->second) {
            res.insert(emitter_entry.first);
        }
        return res;
    }

    std::unordered_set<listener*> listeners_of(const emitter* e, event_type t) const {
        std::unordered_set<listener*> res;
        const auto i = _e.find(const_cast<emitter*>(e));
        if (i == _e.end()) {
            return res;
        }
        const auto j = i->second.find(t);
        if (j == i->second.end()) {
            return res;
        }
        for (const auto& priority_entry : j->second) {
            res.insert(priority_entry.second.first);
        }
        return res;
    }

    std::unordered_set<listener*> listeners_of(const emitter* e) const {
        std::unordered_set<listener*> res;
        const auto i = _e.find(const_cast<emitter*>(e));
        if (i == _e.end()) {
            return res;
        }
        for (const auto& event_entry : i->second) {
            for (const auto& priority_entry : event_entry.second) {
                res.insert(priority_entry.second.first);
            }
        }
        return res;
    }

    bool empty() const {
        return _l.empty() && _e.empty();
    }

};

typedef listener_registry::listener_tag listener_tag;

class emitter {
public:

    virtual ~emitter() {
        listener_registry::instance().unlink(this);
    }

    template <typename E>
    void emit(const E& event) {
        listener_registry::instance().emit<E>(this, event);
    }

    std::unordered_set<listener*> listeners(event_type t) const {
        return listener_registry::instance().listeners_of(this, t);
    }

    std::unordered_set<listener*> listeners() const {
        return listener_registry::instance().listeners_of(this);
    }

};

class listener {
public:

    virtual ~listener() {
        mute();
    }

    template <typename L, typename E>
    listener_tag listen(emitter* emitter, listener_handler<L, E> handler, size_t priority = 0) {
        return listener_registry::instance().link(emitter, this, handler, priority);
    }

    /**
     * Remove all handlers by emitter
     * @param e emitter
     */
    void mute(emitter* e) {
        listener_registry::instance().unlink(this, e);
    }

    /**
     * Remove single handler by returned tag
     * @param t tag
     */
    void mute(const listener_tag& t) {
        listener_registry::instance().unlink(t);
    }

    /**
     * Removes all handlers of this listener
     */
    void mute() {
        listener_registry::instance().unlink(this);
    }

    std::unordered_set<emitter*> emitters() const {
        return listener_registry::instance().emitters_of(this);
    }


};

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


TEST_CASE("listener") {

    test_listener l1;
    test_listener l2;

    emitter e1;
    emitter e2;

    SECTION("before tests registry must be empty") {
        REQUIRE(listener_registry::instance().empty());
    }

    SECTION("can listen for events") {

        l1.listen(&e1, &test_listener::render);
        e1.emit(render_event());
        e1.emit(process_event());

        REQUIRE(l1.render_calls == 1);
        REQUIRE(l1.process_calls == 0);

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
        e2.emit(process_event());

        REQUIRE(l1.render_calls == 1);
        REQUIRE(l1.process_calls == 1);

        REQUIRE(l1.emitters() == std::unordered_set<emitter*>({&e1, &e2}));

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
        REQUIRE(listener_registry::instance().empty());

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
        e2.emit(process_event());

        REQUIRE( l1.render_calls == 0 );
        REQUIRE( l1.process_calls == 1 );

        REQUIRE( l1.emitters() == std::unordered_set<emitter*>({ &e2 }));

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
        REQUIRE(listener_registry::instance().empty());

    }

    SECTION("after tests registry must be empty") {
        REQUIRE(listener_registry::instance().empty());
    }


}