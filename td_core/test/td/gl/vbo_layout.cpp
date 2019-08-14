#include <catch2/catch.hpp>
#include <td/gl/vbo_layout.h>


TEST_CASE("vbo_layout") {

    const td::vbo_layout test_layout = td::vbo_layout().f(3).ub(2).i(4);

    SECTION("offsets and stride") {
        REQUIRE( test_layout.offset(0) == 0 );
        REQUIRE( test_layout.offset(1) == sizeof(GLfloat) * 3 );
        REQUIRE( test_layout.offset(2) == sizeof(GLfloat) * 3 + sizeof(GLubyte) * 2 );
        REQUIRE( test_layout.stride() == sizeof(GLfloat) * 3 + sizeof(GLubyte) * 2 + sizeof(GLint) * 4 );
    }

    SECTION("count") {
        REQUIRE( test_layout.count(0) == 3 );
        REQUIRE( test_layout.count(1) == 2 );
        REQUIRE( test_layout.count(2) == 4 );
    }

    SECTION("type") {
        REQUIRE( test_layout.type(0) == GL_FLOAT );
        REQUIRE( test_layout.type(1) == GL_UNSIGNED_BYTE );
        REQUIRE( test_layout.type(2) == GL_INT );
    }

    SECTION("size") {
        REQUIRE( test_layout.size(0) == sizeof(GLfloat) * 3 );
        REQUIRE( test_layout.size(1) == sizeof(GLubyte) * 2 );
        REQUIRE( test_layout.size(2) == sizeof(GLint) * 4 );
    }

}
