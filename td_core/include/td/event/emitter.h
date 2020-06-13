#pragma once

#include "listener_registry.h"

namespace td {

class emitter {
public:
    virtual ~emitter();

    void emit(const event& ev);
    std::unordered_set<listener*> listeners(event_type t) const;
    std::unordered_set<listener*> listeners() const;

    void activation(event_type t, const listener_registry::activation_fn& fn);
    void remove_activation(event_type t);

};

}