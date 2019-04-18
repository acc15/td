#include <td/buffer/vbo_layout.h>

namespace td {

vbo_layout& vbo_layout::add(GLenum type, size_t count) {
    _items.push_back({ type, count, stride() + gl_sizeof(type) * count });
    return *this;
}

size_t vbo_layout::items() const {
    return _items.size();
}

size_t vbo_layout::offset(size_t index) const {
    return index == 0 ? 0 : _items[index - 1].offset;
}

const void* vbo_layout::ptr(size_t index) const {
    return reinterpret_cast<const void*>(offset(index));
}

size_t vbo_layout::size(size_t index) const {
    const vbo_item& item = _items[index];
    return gl_sizeof(item.type) * item.count;
}

GLint vbo_layout::count(size_t index) const {
    return _items[index].count;
}

GLenum vbo_layout::type(size_t index) const {
    return _items[index].type;
}

GLsizei vbo_layout::stride() const {
    return _items.empty() ? 0 : _items.back().offset;
}

void vbo_layout::disable() const {
    for (size_t i = 0; i < _items.size(); i++) {
        glDisableVertexAttribArray(i);
    }
}

void vbo_layout::enable() const {
    for (size_t i = 0; i < _items.size(); i++) {
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, count(i), type(i), false, stride(), ptr(i));
    }
}



}