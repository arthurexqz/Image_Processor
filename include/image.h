#pragma once

#include <vector>
#include <string>
#include "BMP.h"
#include "pixel.h"

struct Image {
    int height;
    int width;
    std::vector<std::vector<Pixel>> pixels;

    Image() = default;
    Image(int h, int w) : height(h), width(w), pixels(h, std::vector<Pixel>(w)) {
    }
    static Image GetImageFromBMP(const std::string& path);
    static BMP GetBMPHeaderFromImage(const Image&);
    static void WriteBMPToFile(const std::string& path, const Image&);
};