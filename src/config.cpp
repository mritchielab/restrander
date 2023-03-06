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
        return {
            "PCB109",
            "The default configuration. \
            First applies PolyA/PolyT classification, \
            then looks for the standard TSO (SSP) and RTP (VNP) used in PCB109 chemistry.",
            makeDefaultPipeline(),
            false,
            true,
            0.2
        };
    }

    struct Config
    parseConfig(std::string configFilename)
    {
        // open the file
        std::ifstream
        configFile (configFilename);

        // make a new struct
        struct Config config = {};
        auto configJson = nlohmann::json::parse(configFile);
        
        // parse it
        try {
            config.name = configJson["name"];
            config.description = configJson["description"];
            auto pipelineConfig = configJson["pipeline"];
            config.errorRate = configJson["error-rate"];
            config.pipeline = makePipeline(pipelineConfig, config.errorRate);
            config.silent = configJson["silent"];
            config.excludeUnknowns = configJson["exclude-unknowns"];
        } catch (nlohmann::detail::type_error const&) {
            // catch any errors in file parsing
            std::cout << "Error found in config file!\nReverting to default configuration.";
            return makeDefaultConfig();
        } catch (nlohmann::detail::parse_error const&) {
            return makeDefaultConfig();
        }

        return config;
    }



    /*
        reads the config file, and builds a pipeline 
        of operations to perform on each read
    */
    Pipeline
    makePipeline(nlohmann::json pipelineJson, const double errorRate)
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
                    _1, errorRate, method["tso"], method["rtp"], report_artefacts
                );
                pipeline.push_back(function);
            } else if (method["type"] == "primer-extended") {
                // todo
            }

            } catch (nlohmann::detail::type_error const&) {
                std::cout << "Error in configuration file!\nUsing default PCB109 configuration.\n";
                return makeDefaultPipeline();
            }
        }

        return pipeline;
    }
}