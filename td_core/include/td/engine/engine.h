#pragma once

#include "../event/emitter.h"

namespace td {

class engine : public emitter {
public:
    static engine& get();
    engine();
    virtual ~engine();

private:
    static engine* _instance;

};

}