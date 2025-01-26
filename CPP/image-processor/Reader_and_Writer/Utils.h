#ifndef CPP_HSE_UTILS_H
#define CPP_HSE_UTILS_H

#include <string>
#include <vector>
#include <iostream>

namespace image::utils {
    const int BMP_HEADER_SIZE = 14;
    const int DIB_HEADER_SIZE = 40;
    const int PADDING_SIZE = 4;
    const int BYTES_PER_PIXEL = 3;
    const int HEADER_WIDTH_OFFSET = 4;
    const int HEADER_HEIGHT_OFFSET = 8;
    const int FORMAT_FIRST_POSITION = 0;
    const int FORMAT_SECOND_POSITION = 1;
    const int SIZE_POSITION = 2;
    const int PIXES_POSITION = 10;
    const int DIB_HEADER_SIZE_POSITION = 0;
    const int CNT_OF_PLANES = 1;
    const int CNT_OF_PLANES_POSITION = 12;
    const int BITS_PER_PIXEL = 24;
    const int BITS_PER_PIXEL_POSITION = 14;

    const std::vector<char> FORMAT_BYTES = {'B', 'M'};
    const std::vector<int> OFFSETS_OF_BYTES = {8, 16, 24};
};  // namespace image::utils

#endif  // CPP_HSE_UTILS_H
