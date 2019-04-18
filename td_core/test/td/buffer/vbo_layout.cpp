#include <catch2/catch.hpp>
#include <td/buffer/vbo_layout.h>

TEST_CASE("must correctly compute offsets", "[vbo_layout]" ) {

    td::vbo_layout v;

    v.add(GL_FLOAT, 3);
    v.add(GL_UNSIGNED_BYTE, 2);
    v.add(GL_INT, 4);

    REQUIRE( v.offset(0) == 0 );
    REQUIRE( v.offset(1) == sizeof(GLfloat) * 3 );
    REQUIRE( v.offset(2) == sizeof(GLfloat) * 3 + sizeof(GLubyte) * 2 );
    REQUIRE( v.stride() == sizeof(GLfloat) * 3 + sizeof(GLubyte) * 2 + sizeof(GLint) * 4 );

}