#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <string>
#include <vector>
#include <functional>

#include "json.hpp"

using Strand = char;
const Strand forwardStrand = '+';
const Strand reverseStrand = '-';
const Strand unknownStrand = '?';

using Method = std::function<Strand(std::string&)>;
using Pipeline = std::vector<Method>;

Pipeline
makeDefaultPipeline();

Strand
classifyPipeline(std::string& seq, Pipeline& pipeline);

Strand
classifyPoly(std::string& seq, int tailLength, int searchSize);

Strand
classifyPrimer(std::string& seq, int editDistance, std::string ssp, std::string vnp);

#endif