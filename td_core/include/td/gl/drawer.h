#pragma once

#include <fmt/format.h>

#include "vbo_layout.h"
#include "program.h"
#include "sl_uniform_applier.h"
#include "sl_attribute_applier.h"
#include "sl_applier_common.h"
#include "../util/plain_arithmetic_container.h"
#include "bo.h"

namespace td {


class drawer {
public:

    // only external references accepted
    // this is intentional to avoid compiling program every draw operation
    explicit drawer(td::program& p);

    template <typename T>
    drawer& uniform(sl_var_ref ref, const std::initializer_list<T>& value) {
        return uniform(ref, std::vector<T>(value));
    }

    template <typename T>
    drawer& uniform(sl_var_ref ref, const T& value) {
        typedef plain_arithmetic_container_caster<T> caster;
        typedef sl_uniform_applier< typename caster::type > applier;
        sl_apply_value<applier>( _p, _p.uniform(ref), caster::apply(value) );
        return *this;
    }

    template <typename T>
    drawer& uniform(sl_var_ref ref, const T& v1, const T& v2) {
        return uniform( ref, std::array<T, 2>({ v1, v2 }) );
    }

    template <typename T>
    drawer& uniform(sl_var_ref ref, const T& v1, const T& v2, const T& v3) {
        return uniform( ref, std::array<T, 3>({ v1, v2, v3 }) );
    }

    template <typename T>
    drawer& uniform(sl_var_ref ref, const T& v1, const T& v2, const T& v3, const T& v4) {
        return uniform( ref, std::array<T, 4>({ v1, v2, v3 }) );
    }

    template <typename T>
    drawer& attribute(sl_var_ref ref, const T& value) {
        typedef plain_arithmetic_container_caster<T> caster;
        typedef sl_attribute_applier< typename caster::type > applier;

        const sl_var& v = _p.uniform(ref);
        glDisableVertexAttribArray(v.location);
        sl_apply_value<applier>( _p, _p.uniform(ref), caster::apply(value) );

        return *this;
    }

    template <typename T>
    drawer& attribute(sl_var_ref ref, const std::initializer_list<T>& value) {
        return attribute(ref, std::vector<T>(value));
    }

    template <typename T>
    drawer& attribute(sl_var_ref ref, const T& v1, const T& v2) {
        return attribute( ref, std::array<T, 2>({ v1, v2 }) );
    }

    template <typename T>
    drawer& attribute(sl_var_ref ref, const T& v1, const T& v2, const T& v3) {
        return attribute( ref, std::array<T, 3>({ v1, v2, v3 }) );
    }

    template <typename T>
    drawer& attribute(sl_var_ref ref, const T& v1, const T& v2, const T& v3, const T& v4) {
        return attribute( ref, std::array<T, 4>({ v1, v2, v3 }) );
    }

    drawer& attribute(sl_var_ref ref, const td::vbo<>& vbo, uintptr_t offset, GLenum type, GLint size, GLsizei stride);
    drawer& attribute(sl_var_ref ref, const td::vbo<>& vbo, const td::vbo_layout& layout);

    void draw(GLenum mode, GLint offset, GLsizei count);
    void draw(GLenum mode, GLsizei count);
    void draw(GLenum mode);

    /*
    template <typename T>
    drawer& attribute(program::var_ref name, const std::initializer_list<T>& value) {
        return attr(name, std::vector<T>(value));
    }

    template <typename T>
    drawer& attribute(program::var_ref name, const T& value) {
        glVertexAttrib1d(name.attribute_location(_p), 0.0);
        return *this;
    }*/

private:
    program& _p;
    GLsizei _count = -1;

};


}
