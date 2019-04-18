#include <catch2/catch.hpp>
#include <td/buffer/vbo_layout.h>

const td::vbo_layout test_layout = td::vbo_layout().f(3).ub(2).i(4);

TEST_CASE("offsets and stride", "[vbo_layout]" ) {
    REQUIRE( test_layout.offset(0) == 0 );
    REQUIRE( test_layout.offset(1) == sizeof(GLfloat) * 3 );
    REQUIRE( test_layout.offset(2) == sizeof(GLfloat) * 3 + sizeof(GLubyte) * 2 );
    REQUIRE( test_layout.stride() == sizeof(GLfloat) * 3 + sizeof(GLubyte) * 2 + sizeof(GLint) * 4 );
}

TEST_CASE("count", "[vbo_layout]" ) {
    REQUIRE( test_layout.count(0) == 3 );
    REQUIRE( test_layout.count(1) == 2 );
    REQUIRE( test_layout.count(2) == 4 );
}

TEST_CASE("type", "[vbo_layout]" ) {
    REQUIRE( test_layout.type(0) == GL_FLOAT );
    REQUIRE( test_layout.type(1) == GL_UNSIGNED_BYTE );
    REQUIRE( test_layout.type(2) == GL_INT );
}

TEST_CASE("size", "[vbo_layout]" ) {
    REQUIRE( test_layout.size(0) == sizeof(GLfloat) * 3 );
    REQUIRE( test_layout.size(1) == sizeof(GLubyte) * 2 );
    REQUIRE( test_layout.size(2) == sizeof(GLint) * 4 );
}