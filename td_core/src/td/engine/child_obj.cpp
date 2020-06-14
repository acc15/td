#include <td/engine/child_obj.h>

namespace td {

child_obj::~child_obj() {
}

parent_obj* child_obj::parent() const {
    return nullptr;
}

void child_obj::parent(parent_obj* parent) {

}

void child_obj::attach() {

}

void child_obj::detach() {

}


}
