#pragma once

#include <vector>

#include "obj_registry.h"

namespace td {

class parent_obj {
public:
    virtual ~parent_obj();

    void add_child(child_obj* c);

    template <typename T>
    T* child(size_t n) const {
        return dynamic_cast<T*>(obj_registry::get().child(this, n));
    }

    size_t child_count() const;

    std::vector<child_obj*> children() const;

};

}
