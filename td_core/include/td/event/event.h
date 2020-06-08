#pragma once

namespace td {

enum class event_type {
    RENDER,
    PROCESS
};

class event {
public:
    virtual event_type type() const = 0;
};

template<event_type Type>
class generic_event: public event {
public:
    static const event_type TYPE = Type;
    event_type type() const override {
        return TYPE;
    }
};

typedef generic_event<event_type::RENDER> render_event;
class process_event : public generic_event<event_type::PROCESS> {
};


}