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