#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <string>
#include <vector>
#include <functional>


using Strand = char;
const Strand forwardStrand = '+';
const Strand reverseStrand = '-';
const Strand unknownStrand = '?';

using ClassifyFunction = std::function<Strand(std::string*)>;
using Pipeline = std::vector<ClassifyFunction>;

Strand
classifyStrand(std::string* seq, std::string method);

Strand
classifyPipeline(std::string* seq, Pipeline funcs);

Strand
classifyPoly(std::string* seq);

Strand
classifyPrimer(std::string* seq, int editDist);

Strand
classifyPrimerFast(std::string* seq);

Strand
classifyPrimerSlow(std::string* seq);

Strand
classifyPrimerSuperSlow(std::string* seq);

Strand
classifyPrimer4(std::string* seq);

Strand
classifyPrimer6(std::string* seq);

#endif