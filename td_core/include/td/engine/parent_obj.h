#pragma once

#include <vector>

#include "obj_registry.h"

namespace td {

class parent_obj {
public:
    virtual ~parent_obj();

    void add_child(child_obj* c);
    child_obj* child(size_t n);
    size_t child_count() const;

    std::vector<child_obj*> children() const;

};

}
