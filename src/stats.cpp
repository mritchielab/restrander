#include "stats.h"
#include "config.h"

namespace stats {
    nlohmann::json
    toJson(StrandStats strandStats)
    {
        nlohmann::json json;
        for (const auto& [strand, count] : strandStats.stats) {
            json[std::string("") + strand] = count;
        }

        return json;
    }

    nlohmann::json
    toJson(ArtefactStats artefactStats)
    {
        nlohmann::json json;
        for (const auto& [artefact, count] : artefactStats.stats) {
            json[artefact::getName(artefact)] = count;
        }

        return json;
    }

    nlohmann::json
    toJson(Stats stats)
    {
        nlohmann::json json;
        json["totalReads"] = stats.total;
        json["strandStats"] = toJson(stats.strand);
        json["artefactStats"] = toJson(stats.artefact);

        return json;
    }
}