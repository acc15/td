#pragma once

#include <vector>
#include <cstring>

namespace td {

template <typename Element, typename Impl>
class buffer_base {
public:

    typedef Element element_type;

    Impl& reset() {
        _data.resize(0);
        return static_cast<Impl&>(*this);
    }

    Impl& free() {
        reset();
        _data.shrink_to_fit();
        return static_cast<Impl&>(*this);
    }

    const void* data() const {
        return _data.data();
    }

    size_t size() const {
        return count() * sizeof(element_type);
    }

    size_t count() const {
        return _data.size();
    }

    Impl& put(const element_type* src, size_t count) {
        const size_t offset = _data.size();
        _data.resize(offset + count);
        std::memcpy(_data.data() + offset, src, count * sizeof(element_type));
        return static_cast<Impl&>(*this);
    }

private:
    std::vector<element_type> _data;

};


class buffer: public buffer_base<unsigned char, buffer> {
public:
    template <typename T>
    buffer& operator<<(const T& data) {
        return buffer_base<unsigned char, buffer>::put(&data, sizeof(T));
    }
};

template <typename T>
class typed_buffer: public buffer_base<T, typed_buffer<T>> {
public:
    typed_buffer& operator<<(const T& data) {
        return buffer_base<T, typed_buffer<T>>::put(&data, 1);
    }
};


}