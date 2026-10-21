#include <algorithm>
#include "pixel.h"

Pixel Pixel::operator*(float value) const {
    return Pixel{r * value, g * value, b * value};
}

Pixel& Pixel::operator+=(const Pixel& other) {
    r += other.r;
    g += other.g;
    b += other.b;
    return *this;
}