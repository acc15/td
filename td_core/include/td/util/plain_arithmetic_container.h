#pragma once


#include <array>
#include <vector>
#include <Eigen/Dense>


namespace td {

template <typename T, typename E = void>
struct plain_arithmetic_container {
};

template <typename T>
struct plain_arithmetic_container<T, typename std::enable_if< std::is_arithmetic<T>::value >::type> {
    typedef T type;
};

template <typename T>
struct plain_arithmetic_container<T, typename std::enable_if< std::is_array<T>::value >::type> {
    typedef typename plain_arithmetic_container< typename std::remove_all_extents<T>::type >::type type;
};

template <typename T, size_t N>
struct plain_arithmetic_container<std::array<T, N>> {
    typedef typename plain_arithmetic_container<T>::type type;
};

template <typename T, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
struct plain_arithmetic_container<Eigen::Matrix<T, _Rows, _Cols, _Options, _MaxRows, _MaxCols>,
        typename std::enable_if<_Rows != Eigen::Dynamic && _Cols != Eigen::Dynamic>::type> {
    typedef typename plain_arithmetic_container<T>::type type;
};

template <typename T>
struct plain_arithmetic_container_info {
    typedef T type;
    const type* ptr;
    size_t count;
};

template <typename T>
struct plain_arithmetic_container_caster {

    typedef typename plain_arithmetic_container<T>::type type;
    typedef plain_arithmetic_container_info<type> info_type;

    static info_type apply(const T& value) {
        return {
            reinterpret_cast<const type*>(&value),
            sizeof(T) / sizeof(type)
        };
    }
};

template <typename T>
struct plain_arithmetic_container_caster<std::vector<T>> {

    typedef typename plain_arithmetic_container<typename std::vector<T>::value_type>::type type;
    typedef plain_arithmetic_container_info<type> info_type;

    static info_type apply(const std::vector<T>& value) {
        return {
            reinterpret_cast<const type*>(value.data()),
            value.size() * (sizeof(T) / sizeof(type))
        };
    }

};

}

