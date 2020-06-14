
#include <td/engine/tag_registry.h>

namespace td {

tag_registry& tag_registry::get() {
    static tag_registry i;
    return i;
}

std::string tag_registry::tag(const obj* o) const {
    auto o_iter = _o.find(const_cast<obj*>(o));
    return o_iter != _o.end() ? o_iter->second : std::string();
}

void tag_registry::tag(obj* o, const std::string& t) {
    auto o_iter = _o.find(o);
    if (o_iter != _o.end()) {
        if (o_iter->second == t) {
            return;
        }
        _t.erase(o_iter->second);
        o_iter->second = t;
    } else {
        _o[o] = t;
    }

    auto t_iter = _t.find(t);
    if (t_iter != _t.end()) {
        t_iter->second = o;
    } else {
        _t[t] = o;
    }

}

void tag_registry::untag(obj* o) {
    auto o_iter = _o.find(o);
    if (o_iter == _o.end()) {
        return;
    }
    _t.erase(o_iter->second);
    _o.erase(o_iter);
}

obj* tag_registry::by_tag(const std::string& t) const {
    auto t_iter = _t.find(t);
    return t_iter != _t.end() ? t_iter->second : nullptr;
}

}