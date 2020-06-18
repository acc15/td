#include <td/event/event.h>

namespace td {


process_event::process_event(float duration): _duration(duration) {
}

float process_event::duration() const {
    return _duration;
}

}
