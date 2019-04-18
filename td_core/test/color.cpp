#include <catch2/catch.hpp>
#include <td/core/color.h>

using namespace td;


TEST_CASE( "color", "[color_schema]" ) {
    color_bgra v;
    v.set(color_component::RED, 10);
    v.set(color_component::GREEN, 20);
    v.set(color_component::BLUE, 30);
    v.set(color_component::ALPHA, 40);
}