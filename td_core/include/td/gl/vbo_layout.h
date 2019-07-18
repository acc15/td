#pragma once

#include <vector>

#include <td/gl/gl.h>
#include <td/gl/gl_type.h>

namespace td {

class vbo_layout {
public:

    template <typename T>
    vbo_layout& add(size_t count) {
        return add(t2gl<T>::value, count);
    }

    inline vbo_layout& f(size_t count) { return add(GL_FLOAT, count); }
    inline vbo_layout& d(size_t count) { return add(GL_DOUBLE, count); }
    inline vbo_layout& i(size_t count) { return add(GL_INT, count); }
    inline vbo_layout& ui(size_t count) { return add(GL_UNSIGNED_INT, count); }
    inline vbo_layout& b(size_t count) { return add(GL_BYTE, count); }
    inline vbo_layout& ub(size_t count) { return add(GL_UNSIGNED_BYTE, count); }
    inline vbo_layout& s(size_t count) { return add(GL_SHORT, count); }
    inline vbo_layout& us(size_t count) { return add(GL_UNSIGNED_SHORT, count); }

    vbo_layout& add(GLenum type, size_t count);

    size_t items() const;

    size_t offset(size_t index) const;

    /**
     * Offset in bytes for nth attribute
     * @param item attribute index
     * @return offset in bytes of nth item
     */
    const void* ptr(size_t index) const;

    /**
     * Computes amount of bytes required to store nth attribute
     * @param index attribute index
     * @return amount of bytes of nth attribute
     */
    size_t size(size_t index) const;

    /**
     * Returns count of elements of nth attribute
     * @param item item index
     * @return count of elements of nth attribute
     */
    GLint count(size_t index) const;

    /**
     * Returns type of elements of nth attribute
     * @param index index of attribute
     * @return type of element of nth attribute
     */
    GLenum type(size_t index) const;

    /**
     * Computes stride for defined buffer layout
     * @return stride
     */
    GLsizei stride() const;

    void enable() const;
    void disable() const;

private:

    struct vbo_item {
        GLuint location;
        GLenum type;
        size_t count;
        size_t offset;
    };

    std::vector<vbo_item> _items;

};


}