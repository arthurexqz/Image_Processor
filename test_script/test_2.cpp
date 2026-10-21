#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

#include <thread>
#include <chrono>


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

    std::string program = "../build/image_processor"; 
    std::vector<std::string> validInputs = {
        "../test_script/data/lenna.bmp", 
        "../test_script/data/flag.bmp"
    };

    std::cout << "=== Автоматический тест (каждые 5 секунд) ===\n";
    std::cout << "Для остановки нажми Ctrl + C в терминале\n\n";

    int testCounter = 1;

    while (true) {
        std::cout << "----------------------------------------\n";
        std::cout << "Запуск теста #" << testCounter << "\n";

        std::string currentInput = validInputs[rand() % validInputs.size()];

        std::string currentOutput = "../output/output.bmp";
        
        bool allowMist = (rand() % 4 == 0);
        auto filters = GenerateRandomFilters(currentInput, allowMist);
        
        std::string cmd = BuildCommand(program, currentInput, currentOutput, filters);

        std::cout << "[ВХОД]  Файл: " << currentInput << "\n";
        std::cout << "[ВЫХОД] Файл: " << currentOutput << "\n";
        std::cout << "[КОМАНДА]: " << cmd << "\n";
        
        int res = system(cmd.c_str());

        bool expectedFail = false;
        for (auto& f : filters) {
            if (!f.valid) expectedFail = true;
        }

        std::cout << "[РЕЗУЛЬТАТ]: ";
        if ((res != 0 && expectedFail)) {
            std::cout << "Ожидаемая ошибка (плохие параметры).\n";
        } else if ((res == 0 && !expectedFail)) {
            std::cout << "Успех!\n";
        } else {
            std::cout << "ОШИБКА программы (Код: " << res << ").\n";
        }
        
        testCounter++;
        std::cout << "Ждем 5 секунд...\n";
        
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    return 0;
}