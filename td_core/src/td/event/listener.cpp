#include <td/event/listener.h>

namespace td {

listener::~listener() {
    mute();
}

/**
 * Remove all handlers by emitter
 * @param e emitter
 */
void listener::mute(emitter* e) {
    listener_registry::instance().unlink(this, e);
}

/**
 * Remove single handler by returned tag
 * @param t tag
 */
void listener::mute(const listener_tag& t) {
    listener_registry::instance().unlink(t);
}

/**
 * Removes all handlers of this listener
 */
void listener::mute() {
    listener_registry::instance().unlink(this);
}

std::unordered_set<emitter*> listener::emitters() const {
    return listener_registry::instance().emitters_of(this);
}

}