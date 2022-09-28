#include "config.h"

#include <fstream>
#include <iostream>
#include <functional>

#include "json.hpp"
#include "classify.h"

namespace config {
    
    /*
        builds the default configuration for restrander
        (hardcoded in, rather than by loading PCB109.json, 
        in case for some reason that file gets broken in someone's system)
    */
    struct Config
    makeDefaultConfig()
    {
        struct Config config = {};
        config.name = "PCB109";
        config.description = 
            "The default configuration. \
            First applies PolyA/PolyT classification, \
            then looks for the standard TSO (SSP) and RTP (VNP) used in PCB109 chemistry.";
        config.pipeline = makeDefaultPipeline();
        config.silent = false;
        config.excludeUnknowns = true;

        return config;
    }

    struct Config
    parseConfig(std::string configFilename)
    {
        // open the file
        std::ifstream
        configFile (configFilename);

        // make a new struct
        struct Config config = {};
        
        // parse it
        try {
            config.name = nlohmann::json::parse(configFile)["name"];
            config.description = nlohmann::json::parse(configFile)["description"];
            config.pipeline = makePipeline(nlohmann::json::parse(configFile)["pipeline"]);
            config.silent = nlohmann::json::parse(configFile)["silent"];
            config.excludeUnknowns = nlohmann::json::parse(configFile)["exclude-unknowns"];
        } catch (nlohmann::detail::type_error const&) {
            // catch any errors in file parsing
            std::cout << "Error found in config file! Reverting to default configuration.";
            return makeDefaultConfig();
        }
    }

    /*
        reads the config file, and builds a pipeline 
        of operations to perform on each read
    */
    Pipeline
    makePipeline(nlohmann::json pipelineJson)
    {
        using namespace std::placeholders;

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

                // optional argument
                bool report_artefacts = 
                    (method.contains("report-artefacts") ? (bool) method["report-artefacts"] : false);
                
                Method function = std::bind(classifyPrimer, 
                    _1, method["edit-distance"], method["tso"], method["rtp"], report_artefacts
                );
                pipeline.push_back(function);
            }

            } catch (nlohmann::detail::type_error const&) {
                std::cout << "Error in configuration file!\nUsing default PCB109 configuration.\n";
                return makeDefaultPipeline();
            }
        }

        return pipeline;
    }
}