#pragma once

struct Pixel {
    float r, g, b;
    explicit Pixel(float r = 0, float g = 0, float b = 0) : r(r), g(g), b(b) {
    }
    Pixel operator*(float value) const;
    friend Pixel operator*(float value, const Pixel& p) {
        return p * value;
    }
    Pixel& operator+=(const Pixel& other);
};