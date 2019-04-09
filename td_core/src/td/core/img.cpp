#include <memory>

//
// Created by acc15 on 28.03.19.
//

#include <td/core/img.h>

#include <stdexcept>

#include <png.h>
#include <bits/unique_ptr.h>

namespace td {

img::color_type png_color_type_to_img_color_type(png_byte ct) {
    switch (ct) {
        case PNG_COLOR_TYPE_GRAY:
            return img::color_type::GRAY;

        case PNG_COLOR_TYPE_GRAY_ALPHA:
            return img::color_type::GRAY_ALPHA;

        case PNG_COLOR_TYPE_PALETTE:
            return img::color_type::PALLETE;

        case PNG_COLOR_TYPE_RGB:
            return img::color_type::RGB;

        case PNG_COLOR_TYPE_RGB_ALPHA:
            return img::color_type::RGB_ALPHA;

        default:
            return img::color_type::UNKNOWN;
    }
}

void png_read_from_stream(png_structp png_ptr, png_bytep buf, size_t sz) {
    std::istream& in = *static_cast<std::istream*>(png_get_io_ptr(png_ptr));
    in.read(reinterpret_cast<char*>(buf), static_cast<std::streamsize>(sz));
}


img::img() : _width(0), _height(0), _ctype(img::color_type::UNKNOWN), _depth(0), _channels(0), _data(nullptr) {
}


void img::load_png(std::istream &in) {

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        throw std::runtime_error("png_create_read_struct failure");
    }

    png_set_read_fn(png_ptr, static_cast<png_voidp>(&in), png_read_from_stream);

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        throw std::runtime_error("png_create_info_struct failure");
    }

    png_read_info(png_ptr, info_ptr);

    const png_uint_32 row_bytes = png_get_rowbytes(png_ptr, info_ptr);

    _width = png_get_image_width(png_ptr, info_ptr);
    _height = png_get_image_height(png_ptr, info_ptr);
    _depth = png_get_bit_depth(png_ptr, info_ptr);
    _channels = png_get_channels(png_ptr, info_ptr);
    _ctype = png_color_type_to_img_color_type(png_get_color_type(png_ptr, info_ptr));

    _data = std::make_unique<uint8_t[]>(row_bytes * _height);

    std::unique_ptr<png_bytep[]> ptrs = std::make_unique<png_bytep[]>(_height);

    png_bytep data_ptr = _data.get();
    for (size_t i = 0; i < _height; i++) {
        ptrs[i] = data_ptr + (i * row_bytes);
    }

    png_read_image(png_ptr, ptrs.get());
    png_read_end(png_ptr, info_ptr);

    ptrs.reset();

    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);


}

size_t img::height() const {
    return _height;
}

size_t img::width() const {
    return _width;
}

uint8_t img::channels() const {
    return _channels;
}

uint8_t img::depth() const {
    return _depth;
}

img::color_type img::ctype() const {
    return _ctype;
}

uint8_t* img::data() {
    return _data.get();
}


}
