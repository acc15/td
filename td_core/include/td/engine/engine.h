#pragma once

#include <cstddef>
#include <string>

#include <td/engine/fwd.h>
#include <td/engine/event.h>

namespace td {

class engine {
public:
    typedef std::pair<event_type, listener_list::iterator> listener_tag;

    ~engine();

    void attach(engine_object* root);
    engine_object* detach();

    void emit(const event& e);
    void listen(event_type type, engine_object* owner, event_handler handler, size_t priority = 0);
    void mute(event_type type, engine_object* owner);
    void mute(engine_object* owner);
    listener_list& listeners(event_type type);

private:
    engine_object* _root;
    std::map<std::string, engine_object*> _tags;
    std::map<event_type, listener_list> _listeners;

};

}
