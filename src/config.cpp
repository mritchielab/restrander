#include "config.h"

#include <fstream>
#include <iostream>
#include <functional>

#include "json.hpp"
#include "classify.h"

namespace config {
    bool
    isSilent(std::string config)
    {
        std::ifstream
        configFile (config);
        
        try {
            return nlohmann::json::parse(configFile)["silent"];
        } catch (nlohmann::detail::type_error) {
            return false;
        }
    }

    std::string
    getName(std::string config)
    {
        std::ifstream
        configFile (config);
        
        try {
            return nlohmann::json::parse(configFile)["name"];
        } catch (nlohmann::detail::type_error) {
            return "";
        }
    }

    /*
        reads the config file, and builds a pipeline 
        of operations to perform on each read
    */
    Pipeline
    makePipeline(std::string config)
    {
        using namespace std::placeholders;

        std::ifstream
        configFile (config);

        // extract the method section from the config
        auto pipelineJson = nlohmann::json::parse(configFile)["pipeline"];

        Pipeline
        pipeline = {};
        for (const auto & method : pipelineJson) {
            try {
            if (method["type"] == "poly") {
                // here we use bind to partially apply a function
                Method function = std::bind(classifyPoly, 
                    _1, method["tail-length"], method["search-size"]
                );
                pipeline.push_back(function);
            } else if (method["type"] == "primer") {
                Method function = std::bind(classifyPrimer, 
                    _1, method["edit-distance"], method["tso"], method["rt"], method["report-artefacts"]
                );
                pipeline.push_back(function);
            }
            } catch (nlohmann::detail::type_error) {
                std::cout << "Error in configuration file!\nUsing default PCB109 configuration.\n";
                return makeDefaultPipeline();
            }
        }

        return pipeline;
    }
}