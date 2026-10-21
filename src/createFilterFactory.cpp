#include "filters.h"
#include "exceptions.h"
#include <unordered_map>
#include <iostream>
#include <memory>

CreateFilterFactory::CreateFilterFactory() {
    filters_register_ = {{"-crop",
                          [](const std::vector<std::string>& params) {
                              if (params.size() != 2) {
                                  throw AppException(" неправильное кол-во аргументов");
                              }

                              int w = 0;
                              int h = 0;
                              try {
                                  w = std::stoi(params[0]);
                                  h = std::stoi(params[1]);
                              } catch (const std::exception& e) {
                                  throw AppException(" неправильный тип у аргумента/ов");
                              }
                              return std::make_unique<CropFilter>(w, h);
                          }},
                         {"-gs",
                          [](const std::vector<std::string>& params) {
                              if (!params.empty()) {
                                  throw AppException(" неправильное кол-во аргументов");
                              }
                              return std::make_unique<GrayscaleFilter>();
                          }},
                         {"-neg",
                          [](const std::vector<std::string>& params) {
                              if (!params.empty()) {
                                  throw AppException(" неправильное кол-во аргументов");
                              }
                              return std::make_unique<NegativeFilter>();
                          }},
                         {"-sharp",
                          [](const std::vector<std::string>& params) {
                              if (!params.empty()) {
                                  throw AppException(" неправильное кол-во аргументов");
                              }
                              return std::make_unique<SharpeningFilter>();
                          }},
                         {"-edge",
                          [](const std::vector<std::string>& params) {
                              if (params.size() != 1) {
                                  throw AppException(" неправильное кол-во аргументов");
                              }
                              float threshold = 0.0f;
                              try {
                                  threshold = std::stof(params[0]);
                              } catch (const std::exception& e) {
                                  throw AppException(" неправильный тип у аргумента/ов");
                              }
                              return std::make_unique<EdgeDetectionFilter>(threshold);
                          }},
                         {"-blur",
                          [](const std::vector<std::string>& params) {
                              if (params.size() != 1) {
                                  throw AppException(" неправильное кол-во аргументов");
                              }
                              float sigma = 0.0f;
                              try {
                                  sigma = std::stof(params[0]);
                              } catch (const std::exception& e) {
                                  throw AppException(" неправильный тип у аргумента/ов");
                              }
                              return std::make_unique<GaussianBlurFilter>(sigma);
                          }},
                         {"-crystal", [](const std::vector<std::string>& params) {
                              if (params.size() != 1) {
                                  throw AppException(" неправильное кол-во аргументов");
                              }
                              int size = 0;
                              try {
                                  size = std::stoi(params[0]);
                              } catch (const std::exception& e) {
                                  throw AppException(" неправильный тип у аргумента/ов");
                              }
                              return std::make_unique<CICrystalizeFilter>(size);
                          }}};
}

std::unique_ptr<BaseFilter> CreateFilterFactory::Create(const FilterDescriptor& filter_descp) {
    auto it = filters_register_.find(filter_descp.name);
    if (it == filters_register_.end()) {
        throw std::runtime_error("Неизвестный фильтр: " + filter_descp.name);
    }
    try {
        return filters_register_[filter_descp.name](filter_descp.params);
    } catch (const std::exception& e) {
        throw AppException(
            std::string("у фильтра " + filter_descp.name + " неправильно заданы параметры -" + e.what()));
    }
}