#include <td/engine/parent_obj.h>

namespace td {

parent_obj::~parent_obj() {
    obj_registry::get().unlink(this);
}

void parent_obj::add_child(child_obj* c) {
    obj_registry::get().link(c, this);
}

size_t parent_obj::child_count() const {
    return obj_registry::get().child_count(this);
}

std::vector<child_obj*> parent_obj::children() const {
    return obj_registry::get().children(this);
}


}
