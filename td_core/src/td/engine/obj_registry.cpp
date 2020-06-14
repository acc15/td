#include <algorithm>

#include <td/engine/obj_registry.h>
#include <td/engine/parent_obj.h>
#include <td/engine/child_obj.h>

namespace td {

obj_registry& obj_registry::get() {
    static obj_registry i;
    return i;
}

void obj_registry::link(child_obj* c, parent_obj* p) {

    auto p_iter = _p.find(c);
    if (p_iter != _p.end()) {

        if (p_iter->second == p) {
            return;
        }

        auto c_iter = _c.find(p_iter->second);
        if (c_iter == _c.end()) {
            return;
        }

        std::vector<child_obj*>& c_vec = c_iter->second;
        auto cv_iter = std::find(c_vec.begin(), c_vec.end(), c);
        if (cv_iter != c_vec.end()) {
            c_vec.erase(cv_iter);
        }
        if (c_vec.empty()) {
            _c.erase(c_iter);
        }

        if (p == nullptr) {
            _p.erase(p_iter);
            return;
        } else {
            p_iter->second = p;
        }

    } else if (p != nullptr) {
        _p[c] = p;
    } else {
        return;
    }

    _c[p].push_back(c);
}

void obj_registry::unlink(parent_obj* p) {
    auto c_iter = _c.find(p);
    if (c_iter == _c.end()) {
        return;
    }

    for (child_obj* c: c_iter->second) {
        _p.erase(c);
        delete c;
    }

    _c.erase(c_iter);
}

size_t obj_registry::child_count(const parent_obj* p) const {
    auto c_iter = _c.find(const_cast<parent_obj*>(p));
    return c_iter != _c.end() ? c_iter->second.size() : 0;
}

child_obj* obj_registry::child(const parent_obj* p, size_t n) const {
    auto c_iter = _c.find(const_cast<parent_obj*>(p));
    return c_iter != _c.end() ? c_iter->second[n] : nullptr;
}

std::vector<child_obj*> obj_registry::children(const parent_obj* p) const {
    auto c_iter = _c.find(const_cast<parent_obj*>(p));
    return c_iter != _c.end() ? c_iter->second : std::vector<child_obj*>();
}

parent_obj* obj_registry::parent(const child_obj* c) const {
    auto p_iter = _p.find(const_cast<child_obj*>(c));
    return p_iter != _p.end() ? p_iter->second : nullptr;
}

}