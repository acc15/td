#pragma once

#include <cstdint>
#include <cstddef>

#include <istream>
#include <fstream>
#include <memory>

namespace td {


class img {
public:

    enum color_type {
        UNKNOWN,
        GRAY,
        GRAY_ALPHA,
        PALLETE,
        RGB,
        RGB_ALPHA
    };

private:
    size_t _height;
    size_t  _width;
    uint8_t _depth;
    color_type _ctype;
    uint8_t _channels;

    std::unique_ptr<uint8_t[]> _data;


public:

    img();

    void load_png(std::istream &in);

    uint8_t depth() const;
    uint8_t channels() const;
    color_type ctype() const;
    size_t width() const;
    size_t height() const;
    uint8_t* data();

};


}