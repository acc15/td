#pragma once

#include "emitter.h"

namespace td {

class listener {
public:

    virtual ~listener();

    template <typename L, typename E>
    listener_tag listen(emitter* emitter, listener_handler<L, E> handler, size_t priority = 0) {
        return listener_registry::instance().link(
                emitter,
                this,
                E::TYPE,
                reinterpret_cast<listener_handler<listener, event>>(handler),
                priority);
    }

    /**
     * Remove all handlers by emitter
     * @param e emitter
     */
    void mute(emitter* e);

    /**
     * Remove single handler by returned tag
     * @param t tag
     */
    void mute(const listener_tag& t);

    /**
     * Removes all handlers of this listener
     */
    void mute();

    std::unordered_set<emitter*> emitters() const;

};

}