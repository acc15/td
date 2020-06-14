#include <td/engine/child_obj.h>

namespace td {

child_obj::~child_obj() {
    obj_registry::get().link(this, nullptr);
}

parent_obj* child_obj::parent() const {
    return obj_registry::get().parent(this);
}

void child_obj::parent(parent_obj* parent) {
    obj_registry::get().link(this, parent);
}

}
