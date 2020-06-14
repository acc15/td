#include <cassert>

#include <td/engine/engine.h>

namespace td {

engine* engine::_instance = nullptr;

engine* engine::get() {
    assert(_instance != nullptr); // engine not initialized
    return _instance;
}

engine::engine() {
    assert(_instance == nullptr); // this is first engine
    _instance = this;
}

engine::~engine() {
    assert(_instance != nullptr); // engine not initialized
    _instance = nullptr;
}

engine* engine::add(child_obj* c) {
    add_child(c);
    return this;
}



}