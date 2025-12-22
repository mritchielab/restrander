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
        json["artefactBreakdown"] = toJson(stats.breakdown);
        json["totalReads"] = stats.total;

        return json;
    }

    nlohmann::json
    toJson(ArtefactBreakdown breakdown)
    {
        nlohmann::json json;
        for (const auto& [strand, count] : breakdown.noArtefactStrand) {
            json["no artefact"][std::string(1, strand)] = count;
        }
        json["no artefact"]["total"] = breakdown.noArtefactTotal;

        nlohmann::json artefactJson;
        for (const auto& [key, count] : breakdown.artefactTypes) {
            artefactJson[artefact::getName(key)] = count;
        }
        artefactJson["total"] = breakdown.artefactTotal;

        json["artefact"] = artefactJson;

        return json;
    }
}
