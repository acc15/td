#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#undef _USE_MATH_DEFINES

namespace td {

template <typename T>
struct math_const {

    static constexpr T PI = static_cast<T>(M_PI);
    static constexpr T TWO_PI = static_cast<T>(2) * PI;

};

typedef math_const<float> float_math_const;


}

