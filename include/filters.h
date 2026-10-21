#pragma once

#include <string>
#include <functional>
#include <memory>
#include <vector>
#include "filterDescriptor.h"
#include "image.h"

class BaseFilter {
public:
    virtual ~BaseFilter() = default;
    virtual void Apply(Image&) = 0;
};

class ColorFilter : virtual public BaseFilter {
public:
    ~ColorFilter() override = default;
};

class GeometricFilter : virtual public BaseFilter {
public:
    ~GeometricFilter() override = default;
};

class MatrixFilter : virtual public ColorFilter {
    std::vector<std::vector<float>> matrix_;

public:
    explicit MatrixFilter(const std::vector<std::vector<float>>& m) : matrix_(m) {
    }
    void Apply(Image&) override;
    ~MatrixFilter() override = default;
};

class CropFilter : public GeometricFilter {
    int width_;
    int height_;

public:
    explicit CropFilter(int w, int h) : width_(w), height_(h) {
    }
    void Apply(Image&) override;
    ~CropFilter() override = default;
};

class GrayscaleFilter : public ColorFilter {
public:
    GrayscaleFilter() = default;
    void Apply(Image&) override;
    ~GrayscaleFilter() override = default;
};

class NegativeFilter : public ColorFilter {
public:
    NegativeFilter() = default;
    void Apply(Image&) override;
    ~NegativeFilter() override = default;
};

class SharpeningFilter : public MatrixFilter {
public:
    inline static constexpr float EdgeWeight = 5.0f;
    static std::vector<std::vector<float>> MakeMatrix() {
        return {{0, -1, 0}, {-1, EdgeWeight, -1}, {0, -1, 0}};
    }
    SharpeningFilter() : MatrixFilter(MakeMatrix()) {
    }
    ~SharpeningFilter() override = default;
};

class EdgeDetectionFilter : public GrayscaleFilter, public MatrixFilter {
    float threshold_;

public:
    inline static constexpr float EdgeWeight = 4.0f;
    static std::vector<std::vector<float>> MakeMatrix() {
        return {{0, -1, 0}, {-1, EdgeWeight, -1}, {0, -1, 0}};
    }
    explicit EdgeDetectionFilter(float t_h)
        : BaseFilter(), GrayscaleFilter(), MatrixFilter(MakeMatrix()), threshold_(t_h) {
    }
    void Apply(Image&) override;
    ~EdgeDetectionFilter() override = default;
};

class GaussianBlurFilter : public ColorFilter {
    float sigma_;

public:
    explicit GaussianBlurFilter(float sig) : sigma_(sig) {
    }
    void Apply(Image&) override;
    ~GaussianBlurFilter() override = default;
};

class CICrystalizeFilter : public ColorFilter {
    int block_size_;

public:
    explicit CICrystalizeFilter(int size) : block_size_(size) {
    }
    void Apply(Image&) override;
    ~CICrystalizeFilter() override = default;
};

using FuncPtr = std::function<std::unique_ptr<BaseFilter>(const std::vector<std::string>& params)>;
class CreateFilterFactory {
private:
    std::unordered_map<std::string, FuncPtr> filters_register_;

public:
    CreateFilterFactory();
    std::unique_ptr<BaseFilter> Create(const FilterDescriptor&);
    ~CreateFilterFactory() = default;
};