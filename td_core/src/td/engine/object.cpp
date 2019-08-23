#include <td/engine/object.h>

namespace td {

engine_object::engine_object(): _children(), _parent(nullptr), _engine(nullptr) {
}

engine_object::~engine_object() {
    std::vector<engine_object*> temp_children = std::move(_children);
    for (engine_object* child: temp_children) {
        delete child;
    }

    if (_engine != nullptr) {
        on_detach();
        mute_all();
    }
    if (_parent != nullptr) {

        std::vector<engine_object*> pc = _parent->_children;
        const auto it = std::find(pc.begin(), pc.end(), this);
        if (it != pc.end()) {
            pc.erase(it);
        }

    }
    _parent = nullptr;

    engine* detach_engine = _engine;
    _engine = nullptr;
    if (detach_engine != nullptr) {
        detach_engine->detach();
    }

}

void engine_object::attach(engine& e, engine_object* parent) {
    detach();
    _parent = parent;
    _engine = &e;

    for (engine_object* child: _children) {
        child->attach(e, this);
    }
    on_attach();
}

void engine_object::detach() {
    if (_engine == nullptr) {
        return;
    }
    for (engine_object* child: _children) {
        child->detach();
    }
    on_detach();
    mute_all();

    _engine = nullptr;
    _parent = nullptr;
}

bool engine_object::is_attached() const {
    return _engine != nullptr;
}

size_t engine_object::children() const {
    return _children.size();
}

engine_object& engine_object::add(engine_object* child) {
    if (child == nullptr) {
        return *this;
    }

    _children.push_back(child);
    if (_engine) {
        child->attach(*_engine, this);
    }
    return *this;
}

void engine_object::remove(size_t index) {
    delete detach(index);
}

engine_object* engine_object::detach(size_t index) {
    engine_object* child = _children[index];
    child->detach();
    _children.erase(_children.begin() + index);
    return child;
}

engine_object* engine_object::child(size_t index) {
    return _children[index];
}

void engine_object::on_attach() {
}

void engine_object::on_detach() {
}

void engine_object::listen(event_type type, event_handler handler, size_t priority) {
    if (_engine == nullptr) {
        return;
    }
    _engine->listen(type, this, handler, priority);
}

void engine_object::mute(event_type type) {
    if (_engine == nullptr) {
        return;
    }
    _engine->mute(type, this);
}

void engine_object::mute_all() {
    if (_engine == nullptr) {
        return;
    }
    _engine->mute(this);
}

engine* engine_object::get_engine() {
    return _engine;
}


}