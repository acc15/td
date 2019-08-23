#pragma once

//#include <vector>
#include <map>
#include <functional>

#include <cstddef>
#include <td/engine/fwd.h>

namespace td {

enum event_type {
    PROCESS,
    RENDER,
    KEYDOWN,
    KEYUP,
    MOUSEMOVE,
    MOUSEUP,
    MOUSEDOWN,
};

class event {
public:
    virtual event_type type() const = 0;
};

class process_event: public event {
public:
    event_type type() const override;
};

class render_event: public event {
public:
    event_type type() const override;
};

template <typename O, typename E>
using generic_event_handler = void (O::*)(const E&);

typedef generic_event_handler<engine_object, event> event_handler;

struct listener_value {
    engine_object* owner;
    event_handler handler;
};

template <typename O, typename E>
inline event_handler eh(generic_event_handler<O, E> h) {
    return reinterpret_cast<event_handler>(h);
}

class listener_list {
public:
    typedef std::function<void(const listener_list&)> callback_fn;

    typedef std::multimap<size_t, listener_value> listener_map;
    typedef listener_map::iterator iterator;

    listener_list();
    ~listener_list();

    void emit(const event& e);
    iterator listen(engine_object* owner, event_handler handler, size_t priority);
    void mute(const iterator& iter);
    void mute(engine_object* owner);

    void on_attach(const callback_fn& fn);
    void on_detach(const callback_fn& fn);

    static void noop(const listener_list& list);

private:
    typedef std::multimap<engine_object*, iterator> object_map;

    callback_fn _attach;
    callback_fn _detach;

    listener_map _listeners;
    object_map _object;

};

}

