#pragma once

#include "obj_registry.h"

namespace td {

class child_obj {
public:
    virtual ~child_obj();

    template <typename T>
    T* parent() const {
        return dynamic_cast<T*>(obj_registry::get().parent(this));
    }

    void parent(parent_obj* parent);

    virtual void attach();
    virtual void detach();

};

}

