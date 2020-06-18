#include <td/gl/drawer.h>

namespace td {

drawer::drawer(td::program& p): _p(p) {
    glUseProgram(_p.id());
}

drawer& drawer::attribute(sl_var_ref ref, const td::vbo<>& vbo, uintptr_t offset, GLenum type, GLint size, GLsizei stride) {
    GLint loc = ref.attribute_location(_p);
    vbo.bind();
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, size, type, GL_FALSE, stride, reinterpret_cast<const void*>(offset));

    GLsizei point_count = vbo.applied_size() / stride;
    _count = std::min(_count < 0 ? point_count : _count, point_count);
    return *this;
}

drawer& drawer::attribute(sl_var_ref ref, const td::vbo<>& vbo, const td::vbo_layout& layout) {
    const vbo_layout::it& i = ref.has_name() ? layout.by_name(ref.name()) : layout.by_loc(ref.location());
    return attribute(ref, vbo, i.offset, i.type, i.count, layout.stride());
}

void drawer::draw(GLenum mode, GLint offset, GLsizei count) {
    glDrawArrays(mode, offset, count);
}

void drawer::draw(GLenum mode, GLsizei count) {
    draw(mode, 0, count);
}

void drawer::draw(GLenum mode) {
    draw(mode, 0, _count);
}

}