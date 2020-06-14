#pragma once

#include "../event/emitter.h"
#include "parent_obj.h"

namespace td {

class engine : public emitter, public parent_obj {
public:
    static engine* get();
    engine();
    ~engine() override;

    engine* add(child_obj* c);

    // TODO use own key codes with GLFW mapping
    bool is_key_pressed(int code) const;
    engine* title(const char* title);

private:
    static engine* _instance;

};

}