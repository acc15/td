#pragma once

#include <cassert>

#include "../event/listener.h"
#include "../event/emitter.h"

#include "child_obj.h"
#include "parent_obj.h"

namespace td {

class obj : public child_obj, public listener, public emitter, public parent_obj {
public:

    template<typename E>
    void forward(emitter *e) {
        assert(e != this); // can't forward events from self (infinite loop)
        listen(e, [this](const E &e) -> void { this->emit(e); });
    }

    virtual ~obj();

    obj *add(child_obj *c);

    std::string tag() const;
    obj *tag(const std::string &tag);
    obj *untag();

    static obj* by_tag(const std::string &tag);

};

}