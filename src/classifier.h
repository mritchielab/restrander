#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <string>
#include <vector>
#include <functional>

using ClassifyFunction = std::function<std::string(std::string*)>;
using Pipeline = std::vector<ClassifyFunction>;

std::string
classifyDirection(std::string* seq, std::string method);

std::string
classifyPipeline(std::string* seq, Pipeline funcs);

std::string
classifyPoly(std::string* seq);

std::string
classifyPrimer(std::string* seq, int editDist);

std::string
classifyPrimerFast(std::string* seq);

std::string
classifyPrimerSlow(std::string* seq);

std::string
classifyPrimerSuperSlow(std::string* seq);

#endif