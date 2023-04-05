#include "colors.h"

namespace colors {
    std::string
    print(std::string text, std::string color) {
        /*
            print some text in a given color
        */
        return color + text + colors::normal;
    }
}