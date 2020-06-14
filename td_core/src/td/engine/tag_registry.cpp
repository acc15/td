
#include <td/engine/tag_registry.h>

namespace td {

std::string tag_registry::tag(obj* o) const {
    return std::string();
}

void tag_registry::tag(obj* o, const std::string& t) {

}

void tag_registry::untag(obj* o) {

}

}