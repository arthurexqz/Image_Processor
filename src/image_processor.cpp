#include <iostream>
#include "parser.h"
#include "image.h"
#include "filters.h"
#include "exceptions.h"

int main(int argc, char** argv) {
    Parser data;
    try {
        data = Parser(argc, argv);
    } catch (const std::exception& e) {
        std::cout << "Ошибка при парсинге: " << e.what() << std::endl;
        return 1;
    }

    Image img;
    try {
        img = Image::GetImageFromBMP(data.GetInputPath());
    } catch (const std::exception& e) {
        std::cout << std::string("Входной файл поломан: ") + e.what();
        return 1;
    }

    CreateFilterFactory fact;
    for (const auto& filter : data.GetFilters()) {
        std::unique_ptr<BaseFilter> obj_filter;
        try {
            obj_filter = fact.Create(filter);
        } catch (const std::exception& e) {
            std::cout << std::string("Проблема с фильтром: ") + e.what();
            return 1;
        }
        obj_filter->Apply(img);
    }
    
    try {
        Image::WriteBMPToFile(data.GetOutputPath(), img);
    } catch (const std::exception& e) {
        std::cout << std::string("Выходной файл поломан: ") + e.what();
        return 1;
    }
}
