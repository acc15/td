#include <catch2/catch.hpp>
#include <td/core/buf.h>

// static buffer (buffer has fixed max size and can't resize, throws buffer_overflow)
// dynamic buffer (dynamically sized buffer of bytes)

TEST_CASE("buf") {
    SECTION("default constructor") {

        td::buf buf;
        REQUIRE( buf.empty() );

    }

    SECTION("init constructor") {

        float data[] = {1.f,2.f,3.f};
        td::buf b(data, sizeof(data));

    }

    SECTION("copy constructor") {

        td::buf b1;
        b1 << 1.f << 2.f << 3.f;
        td::buf b2(b1);

        REQUIRE(b2.size() == b1.size());
        REQUIRE(*reinterpret_cast<float*>(b1.data()) == *reinterpret_cast<float*>(b2.data()));

    }

    SECTION("move constructor") {

        td::buf buf1;
        td::buf buf2;

        buf1 << 1;
        buf2 = std::move(buf1);

        REQUIRE( buf1.size() == 0 );
        REQUIRE( buf2.size() == sizeof(1) );

    }

    SECTION("stream chain") {
        const td::buf b = std::move(td::buf() << 1 << 1.f << 3.0 << "ch");
        REQUIRE( b.size() == sizeof(1) + sizeof(1.f) + sizeof(3.0) + sizeof("ch") );
    }

    SECTION("call chain") {
        const td::buf b = td::buf().put(1).put(1.f).put(3.0).put("ch");
        REQUIRE( b.size() == sizeof(1) + sizeof(1.f) + sizeof(3.0) + sizeof("ch") );
    }
}
