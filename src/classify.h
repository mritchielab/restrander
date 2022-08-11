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
    strand::Strand strand;
    artefact::Artefact artefact;
};


using Method = std::function<Result(std::string&)>;
using Pipeline = std::vector<Method>;

Pipeline
makeDefaultPipeline();

Result
classifyPipeline(std::string& seq, Pipeline& pipeline);

Result
classifyPoly(std::string& seq, int tailLength, int searchSize);

Result
classifyPrimer(std::string& seq, int editDistance, std::string tso, std::string rt, bool classifyArtefacts);

#endif