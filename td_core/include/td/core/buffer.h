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
        std::copy(src, src + count, _data.data() + offset);
        return static_cast<Impl&>(*this);
    }

private:
    std::vector<element_type> _data;

};


class buffer: public buffer_base<unsigned char, buffer> {
public:
    template <typename T>
    buffer& operator<<(const T& data) {
        return buffer_base<unsigned char, buffer>::put(reinterpret_cast<const unsigned char*>(&data), sizeof(T));
    }
};

template <typename T>
class typed_buffer: public buffer_base<T, typed_buffer<T>> {
public:
    typed_buffer& operator<<(const T& data) {
        return buffer_base<T, typed_buffer<T>>::put(&data, 1);
    }
};

template <typename Type = void>
class static_buffer {
public:

    typedef Type element_type;

    static_buffer():  _count(0), _data(nullptr) {
    }

    static_buffer(const Type* data, size_t count): _count(size), _data(data) {
    }

    static_buffer& free() {
        return reset();
    }

    static_buffer& reset() {
        _data = nullptr;
        _count = 0;
        return *this;
    }

    static_buffer& data(const Type* data, size_t count) {
        _data = data;
        _count = count;
        return *this;
    }

    const void* data() const {
        return _data;
    }

    size_t size() const {
        return _count;
    }

    size_t count() const {
        return _count;
    }

private:
    size_t _count;
    const void* _data;

};


}