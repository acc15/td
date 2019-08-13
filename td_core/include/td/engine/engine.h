#pragma once

#include <cstddef>

#include <td/engine/fwd.h>
#include <td/engine/event.h>

namespace td {


typedef void (*event_handler)(const event& e, engine_object* owner);

struct event_listener {

    size_t priority;
    event_handler handler;
    engine_object* owner;

};


class engine {
public:


    void emit(const event& e);


    void listen(event_type type, engine_object* owner, event_handler handler, size_t priority);
    void mute(event_handler handler);
    void mute_by_owner(engine_object* owner);


};



}