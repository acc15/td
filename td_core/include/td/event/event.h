#pragma once

namespace td {

enum class event_type {
    RENDER,
    PROCESS,
    KEY_DOWN,
    KEY_UP,
    KEY_REPEAT
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

template<event_type Type>
class key_event: public generic_event<Type> {
public:
    key_event(int scan_code): _scan(scan_code) {
    }

    int scan_code() const {
        return _scan;
    }

private:
    int _scan;

};

typedef key_event<event_type::KEY_DOWN> key_down_event;
typedef key_event<event_type::KEY_UP> key_up_event;
typedef key_event<event_type::KEY_REPEAT> key_repeat_event;


}