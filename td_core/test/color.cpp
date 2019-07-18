#include <catch2/catch.hpp>
#include <td/core/color.h>

#include <iostream>

using namespace td;

TEST_CASE("Vector4f to Vector3f and vice versa", "[color]" ) {


    Eigen::Vector4f red = { 1.0, 0.0, 0.0, 1.0 };

    Eigen::RowVector3f redW = red.head(3);

    std::cout << redW << std::endl;


}
