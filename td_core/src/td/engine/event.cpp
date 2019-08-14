#include <algorithm>

#include <td/engine/event.h>

namespace td {

event_type process_event::type() const {
    return PROCESS;
}

event_type render_event::type() const {
    return RENDER;
}

void listener_list::emit(const event& e) {
    for (const auto & _listener : _listeners) {
        (_listener.second.owner->*_listener.second.handler)(e);
    }
}

listener_list::iterator listener_list::listen(engine_object* owner, event_handler handler, size_t priority) {
    listener_value l = { owner, handler };
    auto ret = _listeners.insert(std::make_pair(priority, l));
    _object.insert(std::make_pair(owner, ret));
    return ret;
}

void listener_list::mute(const listener_list::iterator& iter) {
    engine_object* owner = iter->second.owner;
    auto iters = _object.equal_range(owner);
    for (; iters.first != iters.second; ++iters.first) {
        if (iters.first->second == iter) {
            _object.erase(iters.first);
            break;
        }
    }
    _listeners.erase(iter);
}

void listener_list::mute(engine_object* owner) {
    auto iters = _object.equal_range(owner);
    for (; iters.first != iters.second; ++iters.first) {
        _listeners.erase(iters.first->second);
    }
    _object.erase(owner);
}

void event_emitter::emit(const td::event& e) {
    if (_listeners.find(e.type()) == _listeners.end()) {
        return;
    }
    _listeners[e.type()].emit(e);
}

event_emitter::listener_tag event_emitter::listen(event_type type, engine_object* owner, event_handler handler, size_t priority) {
    auto iter = _listeners[type].listen(owner, handler, priority);
    return std::make_pair(type, iter);
}

void event_emitter::mute(const listener_tag& tag) {
    if (_listeners.find(tag.first) == _listeners.end()) {
        return;
    }
    _listeners[tag.first].mute(tag.second);
}

void event_emitter::mute(engine_object* owner) {
    for (auto l: _listeners) {
        l.second.mute(owner);
    }
}

}