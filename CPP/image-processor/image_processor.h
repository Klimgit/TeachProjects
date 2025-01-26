#ifndef CPP_HSE_IMAGE_PROCESSOR_H
#define CPP_HSE_IMAGE_PROCESSOR_H

#include "Image/Image.h"
#include "Filters/Filters.h"
#include "Parsing/Parser.h"

#include <vector>

namespace image_processing {
    struct FilterToken {
        parser::Token token;
    };

    class ImageProcessor {
    public:
        ImageProcessor() = default;
        Image ApplyFilters(const Image& image, const std::vector<FilterToken>& filter_tokens) const;
    };
}  // namespace image_processing
#endif  // CPP_HSE_IMAGE_PROCESSOR_H
