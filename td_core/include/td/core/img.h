#pragma once

#include <cstdint>
#include <cstddef>

#include <istream>
#include <vector>

namespace td {


class img {
public:

    enum color_type {
        UNKNOWN,
        GRAY,
        GRAY_ALPHA,
        PALLETE,
        RGB,
        RGBA
    };

private:
    size_t  _width;
    size_t _height;
    uint8_t _depth;
    color_type _ctype;
    uint8_t _channels;

    std::vector<uint8_t> _data;


public:

    img();

    void load_png(std::istream &in);

    uint8_t depth() const;
    uint8_t channels() const;
    color_type ctype() const;
    size_t width() const;
    size_t height() const;
    const uint8_t* data() const;

};


}
