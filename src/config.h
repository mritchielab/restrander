#ifndef CONFIG_H
#define CONFIG_H

#include "json.hpp"
#include "classify.h"

namespace config {
    struct Config 
    {
        std::string name;
        std::string description;
        Pipeline    pipeline;
        bool        silent;
        bool        excludeUnknowns;
    };

    struct Config
    makeDefaultConfig();

    struct Config
    parseConfig(std::string configFilename);
    
    Pipeline
    makePipeline(nlohmann::json pipelineJson);
}    


#endif