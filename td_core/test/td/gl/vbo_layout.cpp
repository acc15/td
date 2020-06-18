#include <catch2/catch.hpp>
#include <td/gl/vbo_layout.h>


TEST_CASE("vbo_layout") {

    const td::vbo_layout test_layout = std::move( td::vbo_layout().f1("a1").d2("a2").ui3(1).i4(2) );

    SECTION("duplicate attrs must be ignored") {

        td::vbo_layout l;
        l.f1("test");
        l.f2("test");

        l.f1(1);
        l.f2(1);

        REQUIRE(l.items() == 2);
        REQUIRE(l.by_name("test").count == 1);
        REQUIRE(l.by_loc(1).count == 1);

    }

    SECTION("offsets and stride") {
        REQUIRE( test_layout.item(0).offset == 0 );
        REQUIRE( test_layout.item(1).offset == sizeof(GLfloat) );
        REQUIRE( test_layout.item(2).offset == sizeof(GLfloat) + sizeof(GLdouble) * 2 );
        REQUIRE( test_layout.item(3).offset == sizeof(GLfloat) + sizeof(GLdouble) * 2 + sizeof(GLuint) * 3 );
        REQUIRE( test_layout.stride() == sizeof(GLfloat) + sizeof(GLdouble) * 2 + sizeof(GLuint) * 3  + sizeof(GLint) * 4 );
    }

    SECTION("count") {
        REQUIRE( test_layout.item(0).count == 1 );
        REQUIRE( test_layout.item(1).count == 2 );
        REQUIRE( test_layout.item(2).count == 3 );
        REQUIRE( test_layout.item(3).count == 4 );
    }

    SECTION("type") {
        REQUIRE( test_layout.item(0).type == GL_FLOAT );
        REQUIRE( test_layout.item(1).type == GL_DOUBLE );
        REQUIRE( test_layout.item(2).type == GL_UNSIGNED_INT );
        REQUIRE( test_layout.item(3).type == GL_INT );
    }

    SECTION("size") {
        REQUIRE( test_layout.item(0).size() == sizeof(GLfloat) );
        REQUIRE( test_layout.item(1).size() == sizeof(GLdouble) * 2 );
        REQUIRE( test_layout.item(2).size() == sizeof(GLuint) * 3 );
        REQUIRE( test_layout.item(3).size() == sizeof(GLint) * 4 );
    }

    SECTION("by_name") {
        REQUIRE( test_layout.by_name("a1").type == GL_FLOAT );
        REQUIRE( test_layout.by_name("a2").type == GL_DOUBLE );
    }

    SECTION("by_name throws if unknown name") {
        REQUIRE_THROWS( test_layout.by_name("a3") );
    }

    SECTION("by_loc") {
        REQUIRE( test_layout.by_loc(1).type == GL_UNSIGNED_INT );
        REQUIRE( test_layout.by_loc(2).type == GL_INT );
    }

    SECTION("by_loc throws if unknown loc") {
        REQUIRE_THROWS( test_layout.by_loc(3) );
    }

}
