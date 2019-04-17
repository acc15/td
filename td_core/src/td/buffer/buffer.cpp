#include <td/buffer/buffer.h>

namespace td {

static_buffer::static_buffer(): static_buffer(nullptr, 0) {
}

static_buffer::static_buffer(const void* data, size_t size): _data(data), _size(size) {
}

static_buffer& static_buffer::data(const void* data, size_t size) {
    _data = data;
    _size = size;
    return *this;
}

const void* static_buffer::data() const {
    return _data;
}

size_t static_buffer::size() const {
    return _size;
}

size_t static_buffer::count() const {
    return _size;
}

}