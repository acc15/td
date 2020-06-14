#pragma once

#include <string>
#include <unordered_map>

namespace td {

class obj;

class tag_registry {
public:

    static tag_registry& get();

    std::string tag(const obj* o) const;
    void tag(obj* o, const std::string& t);
    void untag(obj* o);

    obj* by_tag(const std::string& t) const;

private:

    tag_registry() = default;

    std::unordered_map<obj*, std::string> _o;
    std::unordered_map<std::string, obj*> _t;

};

}
