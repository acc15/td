#pragma once

#include <unordered_map>

namespace td {

class parent_obj;
class child_obj;

class obj_registry {
public:
    void link(parent_obj* parent, child_obj* child);

private:
    std::unordered_multimap<parent_obj*, child_obj*> _children;
    std::unordered_map<child_obj*, parent_obj*> _parents;
};

}