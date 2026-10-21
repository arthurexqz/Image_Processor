#include "parser.h"
#include "exceptions.h"

Parser::Parser(int argc, char **argv) {
    if (argc < 3) {
        // printHelp();
        throw NotEnoughArgumentsException();
    }

    input_path_ = argv[1];
    output_path_ = argv[2];

    int i = 3;
    while (i < argc) {
        std::string arg = argv[i];

        if (arg[0] != '-') {
            throw AppException("Ошибка: ожидается фильтр, начинающийся с '-', но получен аргумент фильтра: " + arg);
        }
        i += 1;

        // int expectedParams = it->second;
        // std::vector<std::string> params;
        // if (i + expectedParams >= argc) {
        //     throw std::runtime_error("Ошибка: недостаточно параметров для фильтра " + arg);
        // }

        std::vector<std::string> params;
        while (i < argc && argv[i][0] != '-') {
            params.push_back(argv[i]);
            ++i;
        }

        filters_.push_back({arg, params});
    }
}