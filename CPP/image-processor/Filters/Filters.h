#ifndef CPP_HSE_FILTERS_H
#define CPP_HSE_FILTERS_H

#include <algorithm>
#include <memory>
#include <stdexcept>

#include "../Image/Image.h"
#include "../Parsing/Parser.h"
#include "../Reader_and_Writer/Utils.h"

namespace filter {

    const uint8_t MAX_COLOR = 255;
    const double GRAY_ONE = 0.299;
    const double GRAY_TWO = 0.587;
    const double GRAY_THREE = 0.114;

    class Filter {
    public:
        Filter() = default;
        virtual ~Filter() = default;
        virtual Image Apply(const Image& image) const = 0;

    protected:
        template <typename T>
        std::vector<T> GetPixel(const std::vector<std::vector<T>>& matrix, const Image& image, const std::size_t x,
                                const std::size_t y) const;
    };

    class Sharpening : public Filter {
    public:
        Image Apply(const Image& image) const override;
    };

    class Negative : public Filter {
    public:
        Image Apply(const Image& image) const override;
    };

    class Grayscale : public Filter {
    public:
        Image Apply(const Image& image) const override;
    };

    class Crop : public Filter {
    public:
        Crop(size_t width, size_t height) : width_(width), height_(height) {
        }

        Image Apply(const Image& image) const override;

    private:
        size_t width_;
        size_t height_;
    };

    std::unique_ptr<Filter> CreateFilter(const parser::Token& token);

    uint8_t Clamp(int color, const uint8_t& max_color = MAX_COLOR);

}  // namespace filter

#endif  // CPP_HSE_FILTERS_H
