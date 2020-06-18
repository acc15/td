#include <catch2/catch.hpp>

#include <fmt/format.h>
#include <td/gl/sl_var.h>

TEST_CASE("sl_var") {

    const size_t N = 128;

    std::string str[N];

    for (size_t i = 0; i < N; i++) {
        str[i] = fmt::format("str{}", i);
    }

    std::vector<td::sl_var> vec;
    for (size_t i = 0; i < N; i++) {
        vec.push_back({
            static_cast<GLuint>(i * 4),
            static_cast<GLint>(i * 4 + 1),
            i * 4 + 2,
            static_cast<GLint>(i * 4 + 3), str[i] });
    }

    REQUIRE(vec[0].type.value() == 2);


}