#pragma once

#include <string>
#include <unordered_map>

#include "obj.h"

namespace td {

class tag_registry {
public:

    std::string tag(obj* o) const;
    void tag(obj* o, const std::string& t);
    void untag(obj* o);

private:
    std::unordered_map<obj*, std::string> _o;
    std::unordered_map<std::string, obj*> _t;

};

}
