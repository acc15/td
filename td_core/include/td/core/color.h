#pragma once

#include <array>
#include <type_traits>

namespace td {


template <typename T>
constexpr size_t array_length(const T&) noexcept {
    return std::extent<T>::value;
}

enum class color_component {
    RED,
    GREEN,
    BLUE,
    ALPHA,
    MAX
};

template <typename E, E Max>
class enum_indexer {
public:
    template <typename T>
    constexpr int find_component_index(const T& comps, const E& c, size_t i = 0) {
        return i >= array_length(comps) ? -1 : comps[i] == c ? i : find_component_index(comps, c, i + 1);
    }

    template <typename T>
    constexpr explicit enum_indexer(const T& inp): _indexes() {
        for (size_t i = 0; i < _indexes.size(); i++) {
            _indexes[i] = find_component_index(inp, static_cast<E>(i));
        }
    }

    constexpr int index_of(const E& c) const {
        return _indexes[static_cast<size_t>(c)];
    }

private:
    std::array<int, static_cast<size_t>(Max)> _indexes;
};

struct color_layout_rgb {
    constexpr static const color_component layout[] = {
            color_component::RED,
            color_component::GREEN,
            color_component::BLUE
    };
};

struct color_layout_bgra {
    constexpr static const color_component layout[] = {
            color_component::BLUE,
            color_component::GREEN,
            color_component::RED,
            color_component::ALPHA
    };
};

struct color_layout_rgba {
    constexpr static const color_component layout[] = {
            color_component::RED,
            color_component::GREEN,
            color_component::BLUE,
            color_component::ALPHA
    };
};

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
U color_component_cast(const T& comp, const T& max_value, const U& other_max) {
    return static_cast<U>(comp) * max_value / other_max;
}


template <typename Storage>
struct color: public Storage {

    typedef typename Storage::type type;
    typedef typename Storage::layout layout;

    constexpr static const size_t num_components = array_length(layout::layout);
    constexpr static const enum_indexer<color_component, color_component::MAX> indexer =
            enum_indexer<color_component, color_component::MAX>(layout::layout);

    constexpr static int index_of(color_component comp) {
        return indexer.index_of(comp);
    }

    color(): Storage() {
    }

    template <typename T>
    explicit color(const color<T>& other) {
        assign(other);
    }

    template <typename T>
    color<Storage>& operator=(const color<T>& other) {
        assign(other);
        return *this;
    }

    template <typename T>
    void assign(const color<T>& other) {
        for (size_t i = 0; i < Storage::size(); i++) {
            color_component c = layout::layout[i];
            Storage::set_raw(i, other.template get<type>(c, Storage::get_limit(c)));
        }
    }

    constexpr size_t size() const {
        return num_components;
    }

    type get(color_component c) {
        int i = index_of(c);
        return i < 0 ? Storage::get_limit(c) : Storage::get_raw(i);
    }

    void set(color_component c, const type& value) {
        int i = index_of(c);
        if (i < 0) {
            return;
        }
        Storage::set_raw(i, value);
    }

    template <typename U>
    U get(color_component c, const U& max_value) {
        int i = index_of(c);
        return i < 0 ? max_value : color_component_cast<type, U>(Storage::get_raw(i), Storage::get_limit(c), max_value);
    }

    template <typename U>
    void set(color_component c, const U& value, const U& max_value) {
        int i = index_of(c);
        if (i < 0) {
            return;
        }
        set_raw(i, color_component_cast<U, type>(value, max_value, Storage::get_limit(c)));
    }

};

template<typename Type, typename Layout, typename Limits = type_color_component_limits<Type>>
struct array_color {

    typedef Layout layout;
    typedef Type type;

    constexpr static type get_limit(color_component c) {
        return Limits::max();
    }

    type get_raw(size_t idx) const {
        return _data[idx];
    }

    void set_raw(size_t idx, type value) {
        _data[idx] = value;
    }

private:
    std::array<type, array_length(layout::layout)> _data;

};

typedef color< array_color<float, color_layout_rgba> > color_f_rgba;
typedef color< array_color<unsigned char, color_layout_rgba> > color_rgba;
typedef color< array_color<unsigned char, color_layout_bgra> > color_bgra;
typedef color< array_color<unsigned char, color_layout_rgb> > color_rgb;

}