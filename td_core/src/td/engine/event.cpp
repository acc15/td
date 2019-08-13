#include <td/engine/event.h>

namespace td {

event_type process_event::type() const {
    return PROCESS;
}

event_type render_event::type() const {
    return RENDER;
}

}