#pragma once

#include <vector>
#include <array>
#include <stdexcept>
#include <type_traits>

namespace td {

template<typename T>
struct buf_traits {
    typedef typename std::conditional<std::is_void<T>::value, unsigned char, T>::type element_type;
};

template<typename T, size_t N>
class static_buf {
public:

    typedef typename buf_traits<T>::element_type element_type;

    static const size_t MAX_SIZE = N;

    size_t size() const {
        return _pos;
    }

    element_type* data() {
        return _data.data();
    }

    const element_type* data() const {
        return _data.data();
    }

protected:
    void put_impl(const T* data, size_t sz) {
        size_t req_sz = _pos + sz;
        if (req_sz > _data.size()) {
            throw std::overflow_error("buffer overflow");
        }
        const auto* d = static_cast<const element_type*>(data);
        std::copy(d, d + sz, _data.data() + _pos);
        _pos = req_sz;
    }

    void reset_impl() {
        _pos = 0;
    }

    void clear_impl() {
        _pos = 0;
    }

private:

    std::array<element_type, N> _data;
    size_t _pos = 0;

};

template<typename T>
class dynamic_buf : public buf_traits<T> {
public:
    typedef typename buf_traits<T>::element_type element_type;

    size_t size() const {
        return _data.size();
    }

    element_type* data() {
        return _data.data();
    }

    const element_type* data() const {
        return _data.data();
    }

protected:
    void put_impl(const T* data, size_t sz) {
        size_t offset = _data.size();
        size_t req_sz = offset + sz;
        if (req_sz > _data.size()) {
            _data.resize(req_sz);
        }
        const auto* d = static_cast<const element_type*>(data);
        std::copy(d, d + sz, _data.data() + offset);
    }

    void reset_impl() {
        _data.resize(0);
    }

    void clear_impl() {
        reset_impl();
        _data.shrink_to_fit();
    }


private:
    std::vector<element_type> _data;

};

template<typename TImpl, typename T, size_t N>
class basic_buf : public std::conditional<N == 0, dynamic_buf<T>, static_buf<T, N>>::type {
public:
    typedef typename std::conditional<N == 0, dynamic_buf<T>, static_buf<T, N>>::type impl_type;

    bool empty() const {
        return impl_type::size() == 0;
    }

    basic_buf() = default;

    TImpl& reset() {
        impl_type::reset_impl();
        return *static_cast<TImpl*>(this);
    }

    TImpl& clear() {
        impl_type::clear_impl();
        return *static_cast<TImpl*>(this);
    }

    TImpl& put(const T* data, size_t size) {
        impl_type::put_impl(data, size);
        return *static_cast<TImpl*>(this);
    }

    template<typename V>
    TImpl& put(const V& data) {
        return put(&data, sizeof(data));
    }

    template<typename V>
    TImpl& operator<<(const V& data) {
        return put(data);
    }

};

template<typename T = void, size_t N = 0>
class buf : public basic_buf<buf<T, N>, T, N> {
};

}