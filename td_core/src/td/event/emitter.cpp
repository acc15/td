#include <td/event/emitter.h>

namespace td {

emitter::~emitter() {
    listener_registry::instance().unlink(this);
}

void emitter::emit(const event& ev) {
    listener_registry::instance().emit(this, ev);
}

std::unordered_set<listener*> emitter::listeners(event_type t) const {
    return listener_registry::instance().listeners_of(this, t);
}

std::unordered_set<listener*> emitter::listeners() const {
    return listener_registry::instance().listeners_of(this);
}


}