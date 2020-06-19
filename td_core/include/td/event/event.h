#pragma once

#include <cstddef>

namespace td {

enum class event_type {
    RENDER,
    PROCESS,
    KEY_DOWN,
    KEY_UP,
    KEY_REPEAT,
    RESIZE
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
public:
    process_event(float duration);
    float duration;
};

class resize_event : public generic_event<event_type::RESIZE> {
public:
    resize_event(size_t w, size_t h);
    size_t width;
    size_t height;
};


template<event_type Type>
class key_event: public generic_event<Type> {
public:
    key_event(int sc): scan_code(sc) {
    }

    int scan_code;

};

typedef key_event<event_type::KEY_DOWN> key_down_event;
typedef key_event<event_type::KEY_UP> key_up_event;
typedef key_event<event_type::KEY_REPEAT> key_repeat_event;


}