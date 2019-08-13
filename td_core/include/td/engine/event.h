#pragma once

namespace td {

enum event_type {
    PROCESS,
    RENDER,
    KEYDOWN,
    KEYUP,
    MOUSEMOVE,
    MOUSEUP,
    MOUSEDOWN,
};

class event {
public:
    virtual event_type type() const = 0;
};

class process_event: event {
public:
    event_type type() const override;
};

class render_event: event {
    event_type type() const override;
};

}

