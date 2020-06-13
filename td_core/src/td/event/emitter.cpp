#include <td/event/emitter.h>

namespace td {

emitter::~emitter() {
    listener_registry::get().unlink(this);
    listener_registry::get().remove_activations(this);
}

void emitter::emit(const event& ev) {
    listener_registry::get().emit(this, ev);
}

std::unordered_set<listener*> emitter::listeners(event_type t) const {
    return listener_registry::get().listeners_of(this, t);
}

std::unordered_set<listener*> emitter::listeners() const {
    return listener_registry::get().listeners_of(this);
}

void emitter::activation(event_type t, const listener_registry::activation_fn& fn) {
    listener_registry::get().activation(this, t, fn);
}

void emitter::activation(const std::unordered_set<event_type>& t, const listener_registry::activation_fn& fn) {
    listener_registry::get().activation(this, t, fn);
}

void emitter::remove_activations() {
    listener_registry::get().remove_activations(this);
}


}