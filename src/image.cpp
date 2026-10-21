#include <iostream>
#include <fstream>
#include <vector>
#include "BMP.h"
#include "image.h"
#include "exceptions.h"

Image Image::GetImageFromBMP(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) {
        throw CantOpenFile(static_cast<std::string>("(input.txt) ") + path);
    }

    BMP bmp_header;
    input.read(reinterpret_cast<char*>(&bmp_header), sizeof(BMP));

    const uint16_t bmp_signature = 0x4D42;
    if (bmp_header.bfType != bmp_signature) {
        throw NotBMP(path);
    }

    input.seekg(bmp_header.bfOffBits, std::ios::beg);

    int h = bmp_header.biHeight;
    int w = bmp_header.biWidth;

    Image img(h, w);
    int baytes_in_row = w * 3 + (4 - w * 3 % 4) % 4;
    std::vector<uint8_t> row(baytes_in_row);

    for (int y = h - 1; y >= 0; --y) {
        input.read(reinterpret_cast<char*>(row.data()), baytes_in_row);
        for (int x = 0; x < w; ++x) {
            uint8_t b = row[x * 3 + 0];
            uint8_t g = row[x * 3 + 1];
            uint8_t r = row[x * 3 + 2];
            const float color_max = 255.f;
            img.pixels[y][x] = Pixel{static_cast<float>(r) / color_max, static_cast<float>(g) / color_max,
                                     static_cast<float>(b) / color_max};
        }
    }
    return img;
}

BMP Image::GetBMPHeaderFromImage(const Image& image) {
    BMP bmp;

    const uint16_t bmp_signature = 0x4D42;
    const uint32_t bi_s = 40;
    const uint32_t bi_c = 24;
    bmp.bfType = bmp_signature;
    bmp.bfReserved1 = 0;
    bmp.bfReserved2 = 0;
    bmp.biSize = bi_s;
    bmp.biWidth = image.width;
    bmp.biHeight = image.height;
    bmp.biPlanes = 1;
    bmp.biBitCount = bi_c;
    bmp.biCompression = 0;

    int baytes_in_row = bmp.biWidth * 3 + (4 - bmp.biWidth * 3 % 4) % 4;
    bmp.biSizeImage = baytes_in_row * image.height;
    bmp.bfOffBits = sizeof(BMP);
    bmp.bfSize = bmp.bfOffBits + bmp.biSizeImage;

    return bmp;
}

void Image::WriteBMPToFile(const std::string& path, const Image& image) {
    BMP bmp = Image::GetBMPHeaderFromImage(image);

    std::ofstream output(path, std::ios::binary);
    if (!output) {
        throw CantOpenFile(static_cast<std::string>("(output.txt) ") + path);
    }

    output.write(reinterpret_cast<char*>(&bmp), sizeof(BMP));

    int baytes_in_row = image.width * 3 + (4 - image.width * 3 % 4) % 4;
    std::vector<uint8_t> row(baytes_in_row, 0);

    for (int y = image.height - 1; y >= 0; --y) {
        for (int x = 0; x < image.width; ++x) {
            const Pixel& px = image.pixels[y][x];
            const float color_max = 255.f;
            row[x * 3 + 0] = static_cast<uint8_t>(std::min(1.f, std::max(0.f, px.b)) * color_max);
            row[x * 3 + 1] = static_cast<uint8_t>(std::min(1.f, std::max(0.f, px.g)) * color_max);
            row[x * 3 + 2] = static_cast<uint8_t>(std::min(1.f, std::max(0.f, px.r)) * color_max);
        }
        output.write(reinterpret_cast<char*>(row.data()), baytes_in_row);
    }
}
