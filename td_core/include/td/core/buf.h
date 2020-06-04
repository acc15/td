#pragma once

#include <vector>
#include <array>
#include <stdexcept>

namespace td {


inline const unsigned char* uc(const void* data) {
    return static_cast<const unsigned char*>(data);
}

template <typename Impl>
class buf_base {
public:

    Impl& put(const void* data, size_t sz) {
        const size_t offset = impl().size();
        impl().ensure_size(offset + sz);

        const unsigned char* b = uc(data);
        std::copy(b, b + sz, impl()._data.data() + offset);
        return impl();
    }

    template <typename T>
    Impl& put(const T& data) {
        return put(&data, sizeof(data));
    }

    template <typename T>
    Impl& operator<<(const T& data) {
        return put(data);
    }

    Impl& reset() {
        impl().free();
        return impl();
    }

    bool empty() const {
        return impl().size() == 0;
    }

    size_t max_size() const {
        return impl()._data.max_size();
    }

    unsigned char* data() {
        return impl()._data.data();
    }

    const unsigned char* data() const {
        return impl()._data.data();
    }

private:

    Impl& impl() {
        return *reinterpret_cast<Impl*>(this);
    }

    const Impl& impl() const {
        return *reinterpret_cast<const Impl*>(this);
    }

};


class buf: public buf_base<buf> {
public:

    buf() = default;

    buf(const void* data, size_t sz): _data(uc(data), uc(data) + sz) {
    }

    size_t size() const {
        return _data.size();
    }

private:
    std::vector<unsigned char> _data;

    void ensure_size(size_t sz) {
        if (sz > _data.size()) {
            _data.resize(sz);
        }
    }

    void free() {
        _data.clear();
        _data.shrink_to_fit();
    }

    friend buf_base;

};

template <size_t Size>
class static_buf: public buf_base<static_buf<Size>> {
public:
    static_buf(): _sz(0) {
    }

    static_buf(const void* data, size_t sz): _sz(std::min(sz, Size)) {
        std::copy(uc(data), uc(data) + _sz, _data.begin());
    }

    static_buf(const static_buf& cp) = default;
    static_buf& operator=(const static_buf& cp) = default;

    static_buf(static_buf&& mv): _data(std::move(mv._data)), _sz(mv._sz) {
        mv._sz = 0;
    }

    static_buf& operator=(static_buf&& mv) {
        _data = std::move(mv._data);
        _sz = mv._sz;
        mv._sz = 0;
        return *this;
    };

    size_t size() const {
        return _sz;
    }

private:
    std::array<unsigned char, Size> _data;
    size_t _sz;

    void ensure_size(size_t sz) {
        if (sz > _data.size()) {
            throw std::overflow_error("buffer overflow");
        }
        _sz = sz;
    }

    void free() {
        _sz = 0;
    }

    friend buf_base<static_buf>;
};


}
