#include <catch2/catch.hpp>
#include <td/core/color.h>

using namespace td;

template <typename T>
constexpr size_t array_length(const T&) {
    return std::extent<T>::value;
}


constexpr const color_component INDEX_RGBA[] = {
    color_component::B,
    color_component::A,
    color_component::R
};

typedef color_schema<array_length(INDEX_RGBA), INDEX_RGBA> color_schema_rgba;


TEST_CASE( "color", "[color_schema]" ) {

    for (size_t i = 0; i < array_length(INDEX_RGBA); i++) {
        color_component comp = INDEX_RGBA[i];
        const size_t actual_index = color_schema_rgba::index_of(comp);

        CHECK(actual_index == i);
    }
}