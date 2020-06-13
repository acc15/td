#pragma once

#include "../event/emitter.h"

namespace td {

class engine : public emitter {

public:
    static engine& get() {
        static engine instance;
        return instance;
    }

private:
    engine() = default;

};

}