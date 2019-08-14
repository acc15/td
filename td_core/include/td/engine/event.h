#pragma once

#include <vector>
#include <map>

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

typedef void (engine_object::*event_handler)(const event& e);

struct event_listener {
    engine_object* owner;
    event_handler handler;
    size_t priority;
};


class event_emitter {
public:
    void emit(const event& e);
    void listen(event_type type, engine_object* owner, event_handler handler, size_t priority = 0);
    //void mute(event_type type, event_handler handler);
    //void mute_by_owner(engine_object* owner);

private:
    std::map<event_type, std::vector<event_listener>> _listeners;

};



}

