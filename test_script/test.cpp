#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

struct FilterCommand {
    std::string name;
    std::vector<std::string> params;
    bool valid;
};

std::vector<FilterCommand> GenerateRandomFilters(const std::string& inputFile, bool allowWrongParams = false) {
    std::vector<FilterCommand> filters;
    std::vector<std::string> baseFilters = {"-crop", "-gs", "-neg", "-sharp", "-edge", "-crystal"};

    if (inputFile.find("flag.bmp") != std::string::npos) {
        baseFilters.push_back("-blur");
    }

    int numFilters = 1 + rand() % 4;
    for (int i = 0; i < numFilters; ++i) {
        FilterCommand f;
        if (rand() % 15 == 0) {
            f.name = "-badfilter" + std::to_string(rand() % 10);
            f.params = {"999", "-1"};
            f.valid = false;
        } else {
            f.name = baseFilters[rand() % baseFilters.size()];
            f.valid = true;

            if (allowWrongParams && rand() % 3 == 0) {
                f.params.push_back("wrong_param");
                f.valid = false;
            } else {
                if (f.name == "-crop") {
                    f.params.push_back(std::to_string(500 + rand() % 1000));
                    f.params.push_back(std::to_string(500 + rand() % 1000));
                } else if (f.name == "-edge") {
                    float threshold = static_cast<float>(rand()) / RAND_MAX * 0.5f;
                    f.params.push_back(std::to_string(threshold));
                } else if (f.name == "-blur") {
                    float sigma = static_cast<float>(rand()) / RAND_MAX * 5.0f;
                    f.params.push_back(std::to_string(sigma));
                } else if (f.name == "-crystal") {
                    int size = 1 + rand() % 50;
                    f.params.push_back(std::to_string(size));
                }
            }
        }
        filters.push_back(f);
    }
    return filters;
}

std::string BuildCommand(const std::string& program, const std::string& input, const std::string& output,
                         const std::vector<FilterCommand>& filters) {
    std::string cmd = program + " " + input + " " + output;
    for (auto& f : filters) {
        cmd += " " + f.name;
        for (auto& p : f.params)
            cmd += " " + p;
    }
    return cmd;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    std::string program = "./image_processor";
    std::string goodInputLenna = "../test_script/data/lenna.bmp";
    std::string goodInputFlag = "../test_script/data/flag.bmp";
    std::string goodOutput = "../output/output.bmp";
    std::string wrongInput = "../test_script/data/nonexistent.bmp";
    std::string wrongOutput = "/invalid_path/output.bmp";
    std::string wrongFormatInput = "../test_script/data/kitty.webp";

    for (int i = 0; i < 10; ++i) {
        std::string inputFile = goodInputLenna;
        bool allowMist = (rand() % 4 == 0) ? true : false;
        auto filters = GenerateRandomFilters(inputFile, allowMist);
        std::string cmd = BuildCommand(program, inputFile, goodOutput, filters);
        std::cout << "Тест " << i + 1 << ": " << cmd << std::endl;
        int res = system(cmd.c_str());
        bool expectedFail = false;
        for (auto& f : filters)
            if (!f.valid)
                expectedFail = true;
        if ((res != 0 && expectedFail)) {
            std::cout << "  -> Результат как ожидалось (фильтры неправильные)" << std::endl;
        } else if ((res == 0 && !expectedFail)) {
            std::cout << "  -> Результат как ожидалось (все параметры корректные)" << std::endl;
        } else {
            std::cout << "  -> ОШИБКА! Результат не совпадает с ожиданиями" << std::endl;
        }
        std::cout << std::endl;
        std::string smth;
        std::cin >> smth;
        if (smth == "stop") {
            break;
        }
    }

    for (int i = 0; i < 10; ++i) {
        std::string inputFile = goodInputFlag;
        bool allowMist = (rand() % 2 == 0) ? true : false;
        auto filters = GenerateRandomFilters(inputFile, allowMist);
        std::string cmd = BuildCommand(program, inputFile, goodOutput, filters);
        std::cout << "Тест " << i + 1 << ": " << cmd << std::endl;
        int res = system(cmd.c_str());
        bool expectedFail = false;
        for (auto& f : filters)
            if (!f.valid)
                expectedFail = true;
        if ((res != 0 && expectedFail)) {
            std::cout << "  -> Результат как ожидалось (фильтры неправильные)" << std::endl;
        } else if ((res == 0 && !expectedFail)) {
            std::cout << "  -> Результат как ожидалось (все параметры корректные)" << std::endl;
        } else {
            std::cout << "  -> ОШИБКА! Результат не совпадает с ожиданиями" << std::endl;
        }
        std::cout << std::endl;
        std::string smth;
        std::cin >> smth;
        if (smth == "stop") {
            break;
        }
    }

    std::cout << "Тест с неверным входным файлом:\n";
    int res_in = system(BuildCommand(program, wrongInput, goodOutput, {{"-gs", {}, true}}).c_str());
    std::cout << "  -> Код возврата: " << res_in << " (ожидаем ошибку)\n\n";

    std::cout << "Тест с неверным выходным файлом:\n";
    int res_out = system(BuildCommand(program, goodInputLenna, wrongOutput, {{"-gs", {}, true}}).c_str());
    std::cout << "  -> Код возврата: " << res_out << " (ожидаем ошибку)\n\n";

    std::cout << "Тест с файлом неверного формата:\n";
    int res_format = system(BuildCommand(program, wrongFormatInput, goodOutput, {{"-gs", {}, true}}).c_str());
    std::cout << "  -> Код возврата: " << res_format << " (ожидаем ошибку)\n\n";
}