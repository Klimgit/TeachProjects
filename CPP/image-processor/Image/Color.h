#ifndef CPP_HSE_COLOR_H
#define CPP_HSE_COLOR_H

#include <cstdint>

struct Color {
    Color() = default;
    Color(uint8_t blue, uint8_t green, uint8_t red);
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;

    void SetColors(uint8_t input_blue, uint8_t input_green, uint8_t input_red);
};
#endif  // CPP_HSE_COLOR_H
