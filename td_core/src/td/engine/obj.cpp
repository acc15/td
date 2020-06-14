#include <td/engine/obj.h>

namespace td {

obj::~obj() {
}

obj* obj::add(child_obj* c) {
    add_child(c);
    return this;
}

std::string obj::tag() const {
    return std::string();
}

obj* obj::tag(const std::string& tag) {

    return this;
}

obj* obj::untag() {
    return this;
}

obj* obj::by_tag(const std::string& tag) {
    return nullptr;
}

}