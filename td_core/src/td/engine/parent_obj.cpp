#include <td/engine/parent_obj.h>

namespace td {

parent_obj::~parent_obj() {
}

void parent_obj::add_child(child_obj* c) {

}

child_obj* parent_obj::child(size_t n) {
    return nullptr;
}

size_t parent_obj::child_count() const {
    return 0;
}

std::vector<child_obj*> parent_obj::children() const {
    return std::vector<child_obj*>();
}


}
