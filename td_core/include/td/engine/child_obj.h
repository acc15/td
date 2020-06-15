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

    /**
     * Called only once per-instance before attach()
     * when object is attaching to first non-null parent.
     */
    virtual void init();

    /**
     * Called when object attached to parent
     */
    virtual void attach();

    /**
     * Called when object parent is set to null.
     * WARNING!!! NOT called in destructor
     */
    virtual void detach();

};

}

