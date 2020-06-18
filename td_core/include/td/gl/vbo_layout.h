#pragma once

#include <vector>
#include <unordered_map>

#include "sl_var_ref.h"

namespace td {

class vbo_layout {
public:

    struct it {
        size_t index;
        GLenum type;
        GLsizei count;
        GLsizei offset;
        GLsizei size() const;
    };

    vbo_layout() = default;
    vbo_layout(const vbo_layout& copy);
    vbo_layout(vbo_layout&& mv) noexcept;

    vbo_layout& operator=(const vbo_layout& copy);
    vbo_layout& operator=(vbo_layout&& mv) noexcept;

    vbo_layout& attr(sl_var_ref var, GLenum type, GLsizei count);

    vbo_layout& f(sl_var_ref var, GLsizei size);
    vbo_layout& f1(sl_var_ref var);
    vbo_layout& f2(sl_var_ref var);
    vbo_layout& f3(sl_var_ref var);
    vbo_layout& f4(sl_var_ref var);

    vbo_layout& d(sl_var_ref var, GLsizei size);
    vbo_layout& d1(sl_var_ref var);
    vbo_layout& d2(sl_var_ref var);
    vbo_layout& d3(sl_var_ref var);
    vbo_layout& d4(sl_var_ref var);

    vbo_layout& i(sl_var_ref var, GLsizei size);
    vbo_layout& i1(sl_var_ref var);
    vbo_layout& i2(sl_var_ref var);
    vbo_layout& i3(sl_var_ref var);
    vbo_layout& i4(sl_var_ref var);

    vbo_layout& ui(sl_var_ref var, GLsizei size);
    vbo_layout& ui1(sl_var_ref var);
    vbo_layout& ui2(sl_var_ref var);
    vbo_layout& ui3(sl_var_ref var);
    vbo_layout& ui4(sl_var_ref var);

    const it& item(size_t n) const;
    size_t items() const;

    const it& by_name(const char* name) const;
    const it& by_loc(GLint loc) const;

    GLsizei stride() const;

private:

    void make_items_vector();

    std::unordered_map<const char*, it> _name_items;
    std::unordered_map<GLint, it> _loc_items;
    std::vector<it*> _items;

};

}