#include <catch2/catch.hpp>
#include <td/core/color.h>

using namespace td;


TEST_CASE( "color", "[color_schema]" ) {


    color_bgra v;
    v.set(color_component::RED, 10);
    v.set(color_component::GREEN, 20);
    v.set(color_component::BLUE, 30);
    v.set(color_component::ALPHA, 40);


    //INFO(v);


    /*
    for (size_t i = 0; i < array_length(INDEX_RGBA); i++) {
        color_component comp = INDEX_RGBA[i];
        const size_t actual_index = color_schema_rgba::index_of(comp);

        CHECK(actual_index == i);
    }*/
}