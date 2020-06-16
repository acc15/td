#pragma once

#include <vector>
#include <string>

#include "../event/emitter.h"
#include "parent_obj.h"

namespace td {

class engine : public parent_obj, public emitter {
public:
    static engine* get();
    engine();
    ~engine() override;

    engine* add(child_obj* c);

    // TODO use own key codes with GLFW mapping
    bool is_key_pressed(int code) const;
    engine* title(const char* title);
    const std::vector<std::string>& launch_args() const;

private:
    static engine* _instance;

};

}