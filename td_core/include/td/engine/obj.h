#pragma once

#include "../event/listener.h"
#include "../event/emitter.h"

#include "child_obj.h"
#include "parent_obj.h"

namespace td {

class obj: public parent_obj, public child_obj, public listener, public emitter {
public:

    template<typename E>
    void forward(emitter& e) {
        listen(e, [](const E& e) -> void { emit(e); });
    }

    obj& add(child_obj& c);

};

}