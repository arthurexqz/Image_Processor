#pragma once

#include <vector>
#include <string>

struct FilterDescriptor {
    std::string name;
    std::vector<std::string> params;

    FilterDescriptor(const std::string& n, const std::vector<std::string>& p) : name(n), params(p) {
    }
};