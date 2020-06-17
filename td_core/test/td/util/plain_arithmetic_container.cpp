#include <catch2/catch.hpp>

#include <iostream>
#include <type_traits>

#include <td/util/plain_arithmetic_container.h>

TEST_CASE("ptr_cast") {


    SECTION("plain array") {

        std::vector<std::array<Eigen::Vector2f, 2>> my_vec;
        my_vec.push_back(std::array<Eigen::Vector2f, 2>({ Eigen::Vector2f(1.f, 2.f), Eigen::Vector2f(3.f, 4.f)}));
        my_vec.push_back(std::array<Eigen::Vector2f, 2>({ Eigen::Vector2f(5.f, 6.f), Eigen::Vector2f(7.f, 8.f)}));

        typedef td::plain_arithmetic_container_caster< typeof(my_vec) > pc;
        td::plain_arithmetic_container_info<float> pp = pc::apply(my_vec);

        for (size_t i = 0; i < pp.count; i++) {
            std::cout << "[" << i << "] = " << pp.ptr[i] << std::endl;
        }



        std::cout << std::boolalpha << "arithmetic: " << std::is_same<typename plain_arithmetic_container<float>::type, float>::value << std::endl
                  << "array: " << std::is_same<typename plain_arithmetic_container<float[3][4]>::type, float>::value << std::endl
                  << "std::array: " << std::is_same<typename plain_arithmetic_container<std::array<float, 5>>::type, float>::value << std::endl
                  << "Eigen::Vector2f: " << std::is_same<typename plain_arithmetic_container<Eigen::Vector2f>::type, float>::value << std::endl
                  << "std::array<Eigen::Vector2f>: " << std::is_same<typename plain_arithmetic_container<std::array<Eigen::Vector2f, 5>>::type, float>::value << std::endl;
    }



}
