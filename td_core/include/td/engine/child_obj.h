#pragma once

#include "obj_registry.h"

namespace td {

class child_obj {
public:
    virtual ~child_obj();

    parent_obj* parent() const;
    void parent(parent_obj* parent);

    // called after parent is set
    virtual void attach();

    // called before parent reset
    virtual void detach();


};

}

