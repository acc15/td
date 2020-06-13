#pragma once

#include "../event/emitter.h"

namespace td {

class engine : public emitter {
public:
    static engine& get();
    engine();
    ~engine() override;

    // TODO use own key codes with GLFW mapping
    bool is_key_pressed(int code) const;
    void title(const char* title);

private:
    static engine* _instance;

};

}