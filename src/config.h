#ifndef CONFIG_H
#define CONFIG_H

#include "json.hpp"
#include "classify.h"

namespace config {
    bool
    isSilent(std::string config);

    std::string
    getName(std::string config);
    
    Pipeline
    makePipeline(std::string config);
}    


#endif