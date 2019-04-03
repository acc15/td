#include <td/core/exception.h>

namespace td {

exception::exception(const std::string& what) : _what(what) {
}

exception::~exception() {
}

const char *exception::what() const noexcept {
    return _what.c_str();
}

}