#include <td/engine/child_obj.h>

namespace td {

child_obj::~child_obj() {
    obj_registry::get().unlink(this);
}

void child_obj::parent(parent_obj* parent) {
    obj_registry::get().link(this, parent);
}

void child_obj::init() {
}

void child_obj::attach() {
}

void child_obj::detach() {
}

}
