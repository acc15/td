#pragma once

#include <vector>

#include <td/core/gl.h>

namespace td {

class vbo_layout {
public:

    vbo_layout& add(GLenum type, size_t count);

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


    void apply() const;

private:

    struct vbo_item {
        GLenum type;
        size_t count;
    };


    std::vector<vbo_item> _items;

};


}