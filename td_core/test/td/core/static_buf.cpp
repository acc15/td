#include <catch2/catch.hpp>
#include <td/core/buf.h>

// static buffer (buffer has fixed max size and can't resize, throws buffer_overflow)
// dynamic buffer (dynamically sized buffer of bytes)

TEST_CASE("static_buf") {
    SECTION("default constructor") {
        td::buf<void, 20> buf;
        REQUIRE( buf.empty() );
    }

    SECTION("copy constructor") {

        td::buf<void, 20> b1;
        b1 << 1.f << 2.f << 3.f;
        td::buf<void, 20> b2(b1);

        REQUIRE(b2.size() == b1.size());
        REQUIRE(*reinterpret_cast<float*>(b1.data()) == *reinterpret_cast<float*>(b2.data()));

    }

    // for static_buf move constructor works as copy-constructor
    // it is trivially copyable

    SECTION("move constructor") {

        td::buf<void, 20> buf1;
        buf1 << 1;
        td::buf<void, 20> buf2(std::move(buf1));

        REQUIRE( buf1.size() == sizeof(1) );
        REQUIRE( buf2.size() == sizeof(1) );

    }

}
