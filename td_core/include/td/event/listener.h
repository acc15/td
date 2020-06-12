#pragma once

#include "emitter.h"
#include "../util/fn_cast.h"

namespace td {

class listener {
private:

    template<typename E>
    std::pair<event_type, listener_registry::handler_fn> to_handler(const std::function<void(const E&)>& fn) {
        event_type t = E::TYPE;
        return std::make_pair(t, [fn](const td::event& e) -> void { fn(reinterpret_cast<const E&>(e)); });
    }

public:

    virtual ~listener();

    template <typename F>
    listener_tag listen(emitter* emitter, const F& handler, size_t priority = 0) {
        auto fn = fn_cast<listener, F>::to_fn(this, handler);
        std::pair<event_type,listener_registry::handler_fn> h = to_handler(fn);
        return listener_registry::instance().link(
                emitter,
                this,
                h.first,
                h.second,
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