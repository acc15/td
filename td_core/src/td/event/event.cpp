#include <td/event/event.h>

namespace td {

process_event::process_event(float d): duration(d) {
}

resize_event::resize_event(size_t w, size_t h): width(w), height(h) {
}


}
