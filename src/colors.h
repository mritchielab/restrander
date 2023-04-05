#ifndef COLORS_H
#define COLORS_H

#include <string>

namespace colors {

    // define the basic colors
    const std::string magenta = "\x1B[35m";
    const std::string normal  = "\x1B[0m";
    const std::string green   = "\x1B[32m";
    const std::string red     = "\x1B[31m";
    const std::string yellow  = "\x1B[33m";

    // define semantically important colors
    const auto warn = yellow;
    const auto good = green;

    std::string
    print(std::string text, std::string color);
}

#endif