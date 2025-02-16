#include <stdint.h>
#include <string.h>

typedef enum FPClass
{
    FFP_ZERO,         // нули
    FFP_DENORMALIZED, // числа в денормализованной форме
    FFP_NORMALIZED,   // числа в нормализованной форме
    FFP_INF,          // бесконечности
    FFP_NAN           // NaN-ы
} FPClass;

enum {
    SIGN_BIT_POSITION = 31,
    EXPONENT_BIT_POSITION = 23,
    EXPONENT_MASK = 0xFF,
    MANTISSA_MASK = 0x7FFFFF
};

FPClass fpclassf(float value, int *psign) {
    uint32_t bits;
    memcpy(&bits, &value, sizeof(bits));
    *psign = (bits >> SIGN_BIT_POSITION) & 1;

    uint32_t exponent = (bits >> EXPONENT_BIT_POSITION) & EXPONENT_MASK;
    uint32_t mantissa = bits & MANTISSA_MASK;

    if (exponent == EXPONENT_MASK) {
        if (mantissa == 0) {
            return FFP_INF;
        }
        *psign = 0;
        return FFP_NAN;
    }

    if (exponent == 0) {
        if (mantissa == 0) {
            return FFP_ZERO;
        }
        return FFP_DENORMALIZED;
    }

    return FFP_NORMALIZED;
}