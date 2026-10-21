#pragma once

#include <iostream>
#include <vector>
#include "filterDescriptor.h"

class Parser {
public:
    Parser() = default;
    Parser(int argc, char* argv[]);
    std::string GetInputPath() const {
        return input_path_;
    }
    std::string GetOutputPath() const {
        return output_path_;
    }
    const std::vector<FilterDescriptor>& GetFilters() const {
        return filters_;
    }
    // void printHelp() const;

private:
    std::string input_path_;
    std::string output_path_;
    std::vector<FilterDescriptor> filters_;
};