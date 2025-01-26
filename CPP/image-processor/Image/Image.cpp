#include "Image.h"

#include <stdexcept>

Image::Image(size_t height, size_t width) : height_(height), width_(width) {
    data_.resize(height, std::vector<Color>(width));
}

Image::Image(const std::vector<std::vector<Color>>& data) : data_(data) {
    height_ = data.size();
    if (height_ > 0) {
        width_ = data[0].size();
    } else {
        width_ = 0;
    }
}

Image::Image(const Image& other) {
    height_ = other.height_;
    width_ = other.width_;
    data_ = other.data_;
}

size_t Image::GetHeight() const {
    return height_;
}

size_t Image::GetWidth() const {
    return width_;
}

Color& Image::GetColor(size_t x, size_t y) {
    CheckHeightAndWidth(x, y);
    return data_[x][y];
}

const Color& Image::GetColor(size_t x, size_t y) const {
    CheckHeightAndWidth(x, y);
    return data_[x][y];
}

void Image::CheckHeightAndWidth(size_t x, size_t y) const {
    if (x >= height_ || y >= width_) {
        throw std::out_of_range("out of image");
    }
}
