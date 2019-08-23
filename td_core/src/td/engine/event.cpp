#include <algorithm>

#include <td/engine/event.h>

namespace td {

event_type process_event::type() const {
    return PROCESS;
}

event_type render_event::type() const {
    return RENDER;
}

void listener_list::noop(const listener_list& l) {
}

listener_list::listener_list(): _attach(&noop), _detach(&noop) {
}

listener_list::~listener_list() {
    if (!_listeners.empty()) {
        _detach(*this);
    }
}

void listener_list::emit(const event& e) {
    for (const auto & _listener : _listeners) {
        (_listener.second.owner->*_listener.second.handler)(e);
    }
}

listener_list::iterator listener_list::listen(engine_object* owner, event_handler handler, size_t priority) {
    const bool was_empty = _listeners.empty();
    listener_value l = { owner, handler };
    auto ret = _listeners.insert(std::make_pair(priority, l));
    _object.insert(std::make_pair(owner, ret));
    if (was_empty) {
        _attach(*this);
    }
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
    if (_listeners.empty()) {
        _detach(*this);
    }
}

void listener_list::mute(engine_object* owner) {
    auto iters = _object.equal_range(owner);
    for (; iters.first != iters.second; ++iters.first) {
        _listeners.erase(iters.first->second);
    }
    _object.erase(owner);
    if (_listeners.empty()) {
        _detach(*this);
    }
}

void listener_list::on_attach(const listener_list::callback_fn& fn) {
    _attach = fn;
}

void listener_list::on_detach(const listener_list::callback_fn& fn) {
    _detach = fn;
}

}