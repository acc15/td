#pragma once

#include <cassert>

#include "../event/listener.h"
#include "../event/emitter.h"

#include "child_obj.h"
#include "parent_obj.h"
#include "tag_registry.h"

namespace td {

class obj : public child_obj, public listener, public emitter, public parent_obj {
public:

    template<typename E>
    void forward(emitter *e) {
        assert(e != this); // can't forward events from self (infinite loop)
        listen(e, [this](const E &e) -> void { this->emit(e); });
    }

    ~obj() override;
    void detach() override;

    obj *add(child_obj *c);

    std::string tag() const;
    obj *tag(const std::string &tag);
    obj *untag();

    template <typename T>
    static T* by_tag(const std::string &tag) {
        return dynamic_cast<T*>(tag_registry::get().by_tag(tag));
    }

    template <typename T>
    static T* by_tag() {
        return dynamic_cast<T*>(tag_registry::get().by_tag(T::TAG));
    }

};

}