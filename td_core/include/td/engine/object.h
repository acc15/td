#pragma once

#include <vector>

#include <td/engine/engine.h>

namespace td {

class engine_object {
public:

    engine_object();
    virtual ~engine_object();

    virtual void init(engine_object* parent) = 0;
    void attach(engine& e, engine_object* parent);
    engine* engine() const;

private:
    std::vector<engine_object*> _children;
    engine* _engine;

};

}