#include <algorithm>
#include <cstdlib>
#include <cmath>
#include "filters.h"

void CropFilter::Apply(Image& image) {
    int new_width = std::min(width_, image.width);
    int new_height = std::min(height_, image.height);

    Image cropped(new_height, new_width);

    for (int y = 0; y < new_height; ++y) {
        for (int x = 0; x < new_width; ++x) {
            cropped.pixels[y][x] = image.pixels[y][x];
        }
    }

    image = std::move(cropped);
}

void GrayscaleFilter::Apply(Image& image) {
    for (int y = 0; y < image.height; ++y) {
        for (int x = 0; x < image.width; ++x) {
            Pixel& c = image.pixels[y][x];
            const float for_red = 0.299f;
            const float for_green = 0.587f;
            const float for_blue = 0.114f;
            float gray = for_red * c.r + for_green * c.g + for_blue * c.b;
            c.r = c.g = c.b = gray;
        }
    }
}

void NegativeFilter::Apply(Image& image) {
    for (int y = 0; y < image.height; ++y) {
        for (int x = 0; x < image.width; ++x) {
            Pixel& c = image.pixels[y][x];
            c.r = 1.0f - c.r;
            c.g = 1.0f - c.g;
            c.b = 1.0f - c.b;
        }
    }
}

void EdgeDetectionFilter::Apply(Image& image) {
    GrayscaleFilter::Apply(image);
    MatrixFilter::Apply(image);
    for (int y = 0; y < image.height; ++y) {
        for (int x = 0; x < image.width; ++x) {
            Pixel& c = image.pixels[y][x];
            if (c.r > threshold_) {
                c.r = c.g = c.b = 1;
            } else {
                c.r = c.g = c.b = 0;
            }
        }
    }
}

void GaussianBlurFilter::Apply(Image& image) {
    int h = image.height;
    int w = image.width;
    std::vector<std::vector<Pixel>> temp = image.pixels;

    int radius = static_cast<int>(3 * sigma_);
    int size = 2 * radius + 1;
    std::vector<float> kernel(size);
    float sigma2 = sigma_ * sigma_;
    float norm = 0.0f;
    const float gaussian_denominator = 2.0f;

    for (int i = -radius; i <= radius; ++i) {
        kernel[i + radius] = std::exp(-(static_cast<float>(i * i)) / (gaussian_denominator * sigma2));
        norm += kernel[i + radius];
    }
    for (float& v : kernel) {
        v /= norm;
    }

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            float sum_r = 0;
            float sum_g = 0;
            float sum_b = 0;
            for (int k = -radius; k <= radius; ++k) {
                int xx = std::clamp(x + k, 0, w - 1);
                Pixel& p = image.pixels[y][xx];
                float weight = kernel[k + radius];
                sum_r += p.r * weight;
                sum_g += p.g * weight;
                sum_b += p.b * weight;
            }
            Pixel new_pi(sum_r, sum_g, sum_b);
            temp[y][x] = {new_pi};
        }
    }

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            float sum_r = 0;
            float sum_g = 0;
            float sum_b = 0;
            for (int k = -radius; k <= radius; ++k) {
                int yy = std::clamp(y + k, 0, h - 1);
                Pixel& p = temp[yy][x];
                float weight = kernel[k + radius];
                sum_r += p.r * weight;
                sum_g += p.g * weight;
                sum_b += p.b * weight;
            }
            Pixel new_pi(sum_r, sum_g, sum_b);
            image.pixels[y][x] = {new_pi};
        }
    }
}

void CICrystalizeFilter::Apply(Image& image) {
    auto& c = image.pixels;
    int h = image.height;
    int w = image.width;

    for (int by = 0; by < h; by += block_size_) {
        for (int bx = 0; bx < w; bx += block_size_) {
            int rx = bx + std::rand() % block_size_;
            int ry = by + std::rand() % block_size_;

            if (rx >= w) {
                rx = w - 1;
            }
            if (ry >= h) {
                ry = h - 1;
            }

            Pixel block_color = c[ry][rx];

            for (int y = by; y < by + block_size_ && y < h; ++y) {
                for (int x = bx; x < bx + block_size_ && x < w; ++x) {
                    c[y][x] = block_color;
                }
            }
        }
    }
}
