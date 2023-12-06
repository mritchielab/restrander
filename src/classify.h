#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <string>
#include <vector>
#include <functional>

#include "json.hpp"
#include "strand.h"
#include "artefact.h"

/* struct to represent the result of one classification */
struct Result
{
    strand::Strand     strand;
    artefact::Artefact artefact;
};

/* struct to represent the details relevant to a classification */
struct MetaResult
{
    bool polyA;
    bool polyT;
    bool tso;
    bool rtp;
    bool artefact;
};

std::string
metaToString(MetaResult result);

using Method = std::function<Result(std::string&)>;
using Pipeline = std::vector<Method>;

Pipeline
makeDefaultPipeline();

Result
classifyPipeline(std::string& seq, Pipeline& pipeline);

Result
classifyPoly(std::string& seq, int tailLength, int searchSize);

Result
classifyPrimer(std::string& seq, const double errorRate, std::string tso, std::string rtp, bool classifyArtefacts);

MetaResult
metaClassifySeq(std::string& seq);

#endif