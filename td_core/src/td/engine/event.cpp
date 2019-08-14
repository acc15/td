#include <algorithm>

#include <td/engine/event.h>

namespace td {

event_type process_event::type() const {
    return PROCESS;
}

event_type render_event::type() const {
    return RENDER;
}

void event_emitter::emit(const td::event& e) {
    std::vector<event_listener>& list = _listeners[e.type()];
    for (event_listener& l: list) {
        (l.owner->*l.handler)(e);
    }
}

bool compare_event_listener(const event_listener& l1, const event_listener& l2) {
    return l1.priority < l2.priority;
}

void event_emitter::listen(event_type type, engine_object* owner, event_handler handler, size_t priority) {
    event_listener l = {owner, handler, priority};

    std::vector<event_listener>& list = _listeners[type];
    const auto it = std::upper_bound(list.begin(), list.end(), l, &compare_event_listener);
    list.insert(it, l);
}

}