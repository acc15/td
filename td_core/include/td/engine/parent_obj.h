#pragma once

#include "obj_registry.h"

namespace td {

class parent_obj {
public:
    virtual ~parent_obj();

    std::unordered_set<child_obj*> children() const;

};

}
