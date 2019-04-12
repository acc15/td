#pragma once

#include <array>
#include <type_traits>

namespace td {

template <typename T>
constexpr size_t array_length(const T&) {
    return std::extent<T>::value;
}

enum class color_component {
    R, G, B, A, MAX
};

template <size_t N>
class color_component_indexer {
public:
    constexpr int find_component_index(const color_component comps[N], color_component c, size_t i = 0) {
        return i >= N ? -1 :
               comps[i] == c ? i : find_component_index(comps, c, i + 1);
    }

    constexpr explicit color_component_indexer(const color_component L[N]): indexes() {
        for (size_t i = 0; i < static_cast<size_t>(color_component::MAX); i++) {
            indexes[i] = find_component_index(L, static_cast<color_component>(i));
        }
    }

    constexpr int index_of(color_component c) const {
        return indexes[static_cast<size_t>(c)];
    }

    int indexes[static_cast<size_t>(color_component::MAX)];

};


template <size_t N, const color_component L[N]>
struct color_schema {

    constexpr static const size_t comp_count = N;
    constexpr static const color_component_indexer<N> indexer = color_component_indexer<N>(L);

    constexpr static int index_of(color_component c) {
        return indexer.index_of(c);
    }

};


constexpr const color_component INDEX_RGBA[4] = {
        color_component::R,
        color_component::G,
        color_component::B,
        color_component::A
};

typedef color_schema<array_length(INDEX_RGBA), INDEX_RGBA> color_schema_rgba;


template <typename T, typename Spec = void>
struct type_color_component_limits {
    constexpr static T max() {
        return std::numeric_limits<T>::max();
    }
};

template <typename T>
struct type_color_component_limits<T, typename std::enable_if<std::is_floating_point<T>::value>::type> {
    constexpr static T max() {
        return static_cast<T>(1.f);
    }
};

template <typename T, typename U>
U color_component_cast(const T& comp, const T& comp_max, const U& result_max) {
    return static_cast<U>(comp) * result_max / comp_max;
}


template<typename T, typename S, typename L = type_color_component_limits<T>>
class color {
public:
    typedef S comp_layout;
    typedef T comp_type;

    constexpr static T get_component_limit(color_component c) {
        return L::max();
    }

    template <typename U>
    U get(color_component c, const U& max_value) {
        int i = comp_layout::index_of(c);
        return i < 0 ? max_value : color_component_cast(_data[i], get_component_limit(c), max_value);
    }

private:
    std::array<T, S::comp_count> _data;

};

}