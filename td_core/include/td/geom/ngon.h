#pragma once

#include <cmath>
#include <Eigen/Dense>

#include "../core/math_const.h"

namespace td {

template <typename T, size_t N>
struct ngon {

    typedef Eigen::Matrix<T, 2, 1> point;
    typedef td::math_const<T> constants;

    static constexpr T PI_DIV_N = constants::TWO_PI / static_cast<T>(N);

    point p[N];

    explicit ngon(const T& r) {
        for (size_t i = 0; i < N; i++) {
            p[i] = point( cos(PI_DIV_N * static_cast<T>(i)) * r, sin(PI_DIV_N * static_cast<T>(i)) * r );
        }
    }

};


}
