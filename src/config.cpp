#include "config.h"

#include <fstream>
#include <iostream>
#include <functional>

#include "json.hpp"
#include "classify.h"
#include "colors.h"
#include "utilities.h"
#include "program.h"

namespace config {
    bool
    pipelineHasArtefacts(Pipeline pipeline)
    {
        // for (const auto& classify : pipeline) {

        // }
        return true;
    }

    
    /*
        builds the default configuration for restrander
        (hardcoded in, rather than loading PCB109.json, 
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

    nlohmann::json
    parseConfigFile(std::string configFilename)
    {
        std::ifstream configFile (configFilename);

        return nlohmann::json::parse(configFile);
    }

    struct Config
    parseConfig(std::string configFilename)
    {

        try {
            // open the file
            std::ifstream
            configFile (configFilename);

            // parse it into json
            auto configJson = parseConfigFile(configFilename);

            // make a new struct
            return (Config) {
                configJson["name"],
                configJson["description"],
                makePipeline(configJson["pipeline"], configJson["error-rate"]),
                configJson["silent"],
                configJson["exclude-unknowns"],
                configJson["error-rate"]
            };
    
        } catch (...) {
            program::error("Error when parsing config file!\nCheck that the path you entered was valid.\n");
        }

        // should never get here
        return makeDefaultConfig();
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
                program::error("Error in configuration file!\n");
            }
        }

        return pipeline;
    }
}