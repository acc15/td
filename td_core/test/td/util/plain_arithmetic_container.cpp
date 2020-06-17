#include <catch2/catch.hpp>

#include <iostream>
#include <type_traits>

#include <td/util/plain_arithmetic_container.h>

TEST_CASE("plain_arithmetic_container") {

    std::vector<std::array<Eigen::Vector2f, 2>> my_vec;
    my_vec.push_back(std::array<Eigen::Vector2f, 2>({ Eigen::Vector2f(1.f, 2.f), Eigen::Vector2f(3.f, 4.f)}));
    my_vec.push_back(std::array<Eigen::Vector2f, 2>({ Eigen::Vector2f(5.f, 6.f), Eigen::Vector2f(7.f, 8.f)}));

    typedef td::plain_arithmetic_container_caster< decltype(my_vec) > pc;
    td::plain_arithmetic_container_info<float> pp = pc::apply(my_vec);

    REQUIRE(pp.ptr[0] == my_vec[0][0][0]);
    REQUIRE(pp.ptr[1] == my_vec[0][0][1]);
    REQUIRE(pp.ptr[2] == my_vec[0][1][0]);
    REQUIRE(pp.ptr[3] == my_vec[0][1][1]);
    REQUIRE(pp.ptr[4] == my_vec[1][0][0]);
    REQUIRE(pp.ptr[5] == my_vec[1][0][1]);
    REQUIRE(pp.ptr[6] == my_vec[1][1][0]);
    REQUIRE(pp.ptr[7] == my_vec[1][1][1]);

    REQUIRE( std::is_same<typename td::plain_arithmetic_container<float>::type, float>::value );
    REQUIRE( std::is_same<typename td::plain_arithmetic_container<float[3][4]>::type, float>::value );
    REQUIRE( std::is_same<typename td::plain_arithmetic_container<std::array<float, 5>>::type, float>::value );
    REQUIRE( std::is_same<typename td::plain_arithmetic_container<Eigen::Vector2f>::type, float>::value );
    REQUIRE( std::is_same<typename td::plain_arithmetic_container<std::array<Eigen::Vector2f, 5>>::type, float>::value );

}
