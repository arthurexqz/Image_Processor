#include <vector>
#include <algorithm>
#include "filters.h"
#include <iostream>

int GetIndex(int idx, int max) {
    if (idx < 0) {
        return 0;
    }
    if (idx >= max) {
        return max - 1;
    }
    return idx;
}

void MatrixFilter::Apply(Image& image) {
    auto& c = image.pixels;
    auto copy_pixels = image.pixels;
    for (int y = 0; y < image.height; ++y) {
        for (int x = 0; x < image.width; ++x) {
            c[y][x] = Pixel(0.f, 0.f, 0.f);
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    int iy = GetIndex(y + dy, image.height);
                    int ix = GetIndex(x + dx, image.width);
                    c[y][x] += matrix_[dy + 1][dx + 1] * copy_pixels[iy][ix];
                }
            }
            c[y][x].r = std::min(1.0f, std::max(0.0f, c[y][x].r));
            c[y][x].g = std::min(1.0f, std::max(0.0f, c[y][x].g));
            c[y][x].b = std::min(1.0f, std::max(0.0f, c[y][x].b));
        }
    }
}