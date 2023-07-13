#include "colors.h"

namespace colors {
    /*
        print some text in a given color
    */
    std::string
    print(std::string text, std::string color) 
    {
        return color + text + colors::normal;
    }
}