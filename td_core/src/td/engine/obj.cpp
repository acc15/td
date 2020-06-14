#include <td/engine/obj.h>
#include <td/engine/tag_registry.h>

namespace td {

obj::~obj() {
    tag_registry::get().untag(this);
}

obj* obj::add(child_obj* c) {
    add_child(c);
    return this;
}

std::string obj::tag() const {
    return tag_registry::get().tag(this);
}

obj* obj::tag(const std::string& tag) {
    tag_registry::get().tag(this, tag);
    return this;
}

obj* obj::untag() {
    tag_registry::get().untag(this);
    return this;
}

obj* obj::by_tag(const std::string& tag) {
    return tag_registry::get().by_tag(tag);
}

}