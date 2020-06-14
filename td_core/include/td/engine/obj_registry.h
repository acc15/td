#pragma once

#include <unordered_map>

namespace td {

class parent_obj;
class child_obj;

class obj_registry {
public:
    static obj_registry& get();

    void link(child_obj* c, parent_obj* p);
    void unlink(parent_obj* p);

    size_t child_count(const parent_obj* p) const;
    child_obj* child(const parent_obj* p, size_t n) const;

    std::vector<child_obj*> children(const parent_obj* p) const;
    parent_obj* parent(const child_obj* c) const;

private:
    obj_registry() = default;

    std::unordered_map<parent_obj*, std::vector<child_obj*>> _c;
    std::unordered_map<child_obj*, parent_obj*> _p;

};

}