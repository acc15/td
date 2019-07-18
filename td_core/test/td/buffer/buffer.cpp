#include <catch2/catch.hpp>
#include <td/core/buffer.h>

TEST_CASE("move ctor", "[static_buffer]" ) {

    size_t data = 123;

    td::static_buffer buf1;
    td::static_buffer buf2;

    buf1.data(&data, sizeof(data));
    buf2 = buf1;

    REQUIRE( buf1.data() == &data );
    REQUIRE( buf2.data() == &data);
    REQUIRE( buf2.size() == buf1.size() );

}

TEST_CASE("buffer has move ctor", "[buffer]" ) {

    size_t data = 123;

    td::buffer buf1;
    td::buffer buf2;

    buf1 << data;

    buf2 = std::move(buf1);

    REQUIRE( buf1.size() == 0 );
    REQUIRE( buf2.size() == sizeof(data) );

}