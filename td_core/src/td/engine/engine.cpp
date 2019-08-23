#include <td/engine/engine.h>
#include <td/engine/object.h>

namespace td {

engine::~engine() {
    // TODO detach
}

void engine::attach(td::engine_object* root) {
    _root = root;
}

void engine::emit(const event& e) {
    if (_listeners.find(e.type()) == _listeners.end()) {
        return;
    }
    _listeners[e.type()].emit(e);
}

void engine::listen(event_type type, engine_object* owner, event_handler handler, size_t priority) {
    _listeners[type].listen(owner, handler, priority);
}

void engine::mute(const event_type type, engine_object* owner) {
    if (_listeners.find(type) == _listeners.end()) {
        return;
    }
    _listeners[type].mute(owner);
}

void engine::mute(engine_object* owner) {
    for (auto l: _listeners) {
        l.second.mute(owner);
    }
}

listener_list& engine::listeners(event_type type) {
    return _listeners[type];
}

engine_object* engine::detach() {
    if (_root == nullptr) {
        return nullptr;
    }

    engine_object* obj = _root;
    _root = nullptr;
    obj->detach();
    return obj;
}

}