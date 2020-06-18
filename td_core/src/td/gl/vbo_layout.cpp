#include <stdexcept>
#include <fmt/format.h>

#include <td/gl/vbo_layout.h>
#include <td/gl/gl_type.h>

namespace td {

vbo_layout::vbo_layout(const vbo_layout& copy): _name_items(copy._name_items), _loc_items(copy._loc_items) {
    make_items_vector();
}

vbo_layout::vbo_layout(vbo_layout&& mv) noexcept :
    _name_items(std::move(mv._name_items)),
    _loc_items(std::move(mv._loc_items)),
    _items(std::move(mv._items))
{
}

vbo_layout& vbo_layout::operator=(const vbo_layout& copy) {
    _name_items = copy._name_items;
    _loc_items = copy._loc_items;
    make_items_vector();
    return *this;
}

vbo_layout& vbo_layout::operator=(vbo_layout&& mv) noexcept {
    _name_items = std::move(mv._name_items);
    _loc_items = std::move(mv._loc_items);
    _items = std::move(mv._items);
    return *this;
}


GLsizei vbo_layout::it::size() const {
    return count * gl_sizeof(type);
}

vbo_layout& vbo_layout::attr(sl_var_ref var, GLenum type, GLsizei count) {
    const it i = { _items.size(), type, count, stride() };

    it* p;
    if (var.has_name()) {
        auto pp = _name_items.emplace(var.name(), i);
        if (!pp.second) {
            return *this;
        }
        p = &pp.first->second;
    } else {
        auto pp = _loc_items.emplace(var.location(), i);
        if (!pp.second) {
            return *this;
        }
        p = &pp.first->second;
    }

    _items.push_back(p);
    return *this;
}

vbo_layout& vbo_layout::f(sl_var_ref var, GLsizei size) {
    return attr(var, GL_FLOAT, size);
}

vbo_layout& vbo_layout::f1(sl_var_ref var) {
    return f(var, 1);
}

vbo_layout& vbo_layout::f2(sl_var_ref var) {
    return f(var, 2);
}

vbo_layout& vbo_layout::f3(sl_var_ref var) {
    return f(var, 3);
}

vbo_layout& vbo_layout::f4(sl_var_ref var) {
    return f(var, 4);
}

vbo_layout& vbo_layout::d(sl_var_ref var, GLsizei size) {
    return attr(var, GL_DOUBLE, size);
}

vbo_layout& vbo_layout::d1(sl_var_ref var) {
    return d(var, 1);
}

vbo_layout& vbo_layout::d2(sl_var_ref var) {
    return d(var, 2);
}

vbo_layout& vbo_layout::d3(sl_var_ref var) {
    return d(var, 3);
}

vbo_layout& vbo_layout::d4(sl_var_ref var) {
    return d(var, 4);
}

vbo_layout& vbo_layout::i(sl_var_ref var, GLsizei size) {
    return attr(var, GL_INT, size);
}

vbo_layout& vbo_layout::i1(sl_var_ref var) {
    return i(var, 1);
}

vbo_layout& vbo_layout::i2(sl_var_ref var) {
    return i(var, 2);
}

vbo_layout& vbo_layout::i3(sl_var_ref var) {
    return i(var, 3);
}

vbo_layout& vbo_layout::i4(sl_var_ref var) {
    return i(var, 4);
}

vbo_layout& vbo_layout::ui(sl_var_ref var, GLsizei size) {
    return attr(var, GL_UNSIGNED_INT, size);
}

vbo_layout& vbo_layout::ui1(sl_var_ref var) {
    return ui(var, 1);
}

vbo_layout& vbo_layout::ui2(sl_var_ref var) {
    return ui(var, 2);
}

vbo_layout& vbo_layout::ui3(sl_var_ref var) {
    return ui(var, 3);
}

vbo_layout& vbo_layout::ui4(sl_var_ref var) {
    return ui(var, 4);
}

const vbo_layout::it& vbo_layout::item(size_t n) const {
    return *_items[n];
}

size_t vbo_layout::items() const {
    return _items.size();
}

const vbo_layout::it& vbo_layout::by_name(const char* name) const {
    auto it = _name_items.find(name);
    if (it == _name_items.end()) {
        throw std::invalid_argument(fmt::format("vbo_layout doesn't have attribute with name {}", name));
    }
    return it->second;
}

const vbo_layout::it& vbo_layout::by_loc(GLint loc) const {
    auto it = _loc_items.find(loc);
    if (it == _loc_items.end()) {
        throw std::invalid_argument(fmt::format("vbo_layout doesn't have attribute with location {}", loc));
    }
    return it->second;
}

GLsizei vbo_layout::stride() const {
    if (_items.empty()) {
        return 0;
    }
    const it* i = _items.back();
    return i->offset + i->size();
}

void vbo_layout::make_items_vector() {
    _items.resize(_name_items.size() + _loc_items.size());
    for (auto& ne: _name_items) {
        _items[ne.second.index] = &ne.second;
    }
    for (auto& le: _loc_items) {
        _items[le.second.index] = &le.second;
    }
}


}