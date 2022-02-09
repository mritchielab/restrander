#include <string>
#include <functional>
#include <unordered_map>

#include "utilities.h"
#include "classifier.h"

/*
    classifies whether a sequence is forward or reverse,
    using a given method
*/
std::string
classifyDirection(std::string* seq, std::string method)
{
    std::unordered_map<std::string, Pipeline> classifiers;
    classifiers["standard"]     = {classifyPoly,classifyPrimerFast};
    classifiers["slow"]         = {classifyPoly,classifyPrimerSlow};
    classifiers["superslow"]    = {classifyPoly,classifyPrimerSuperSlow};
    classifiers["trimmed"]      = {classifyPoly};

    return classifyPipeline(seq, classifiers[method]);
}

/*
    classifies a sequence using a given classification pipeline
*/
std::string
classifyPipeline(std::string* seq, Pipeline funcs)
{
    std::string
    direction = "ambiguous";
    
    for (const auto & classify : funcs) {
        // try this classifier
        direction = classify(seq);

        // if we've successfully classified it, end loop
        if (direction != "ambiguous") {
            break;
        }
    }

    return direction;
}

/*
    classifies a sequence based on the presence of a PolyA or PolyT tail
    works around 99% of the time
*/
std::string
classifyPoly(std::string* seq)
{
    bool polyATail = hasPolyATail(seq);
    bool polyTTail = hasPolyTTail(seq);

    if (polyATail && !polyTTail) {
        return "forward";
    } else if (!polyATail && polyTTail) {
        return "reverse";
    } else if ((polyATail && polyTTail) || (!polyATail && !polyTTail)) {
        return "ambiguous";
    }

    // should never get here
    return "amiguous";
}

/*
    classifies a sequence by looking for the SSP or VNP near the start,
    using a given an edit distance
*/
std::string
classifyPrimer(std::string* seq, int editDist)
{   
    bool SSP = hasSSP(seq, editDist);
    bool VNP = hasVNP(seq, editDist);

    if (SSP && !VNP) {
        return "forward";
    } else if (!SSP && VNP) {
        return "reverse";
    } else if ((!SSP && !VNP) || (SSP && VNP)) {
        return "ambiguous";
    }

    // should never get here
    return "ambiguous";
}

/*
    runs classifyPrimer with a reasonable edit distance,
    runs fairly quickly
*/
std::string
classifyPrimerFast(std::string* seq)
{
    return classifyPrimer(seq, 3);
}

/*
    same as classifyPrimer, but with an increased edit distance
    more accurate but slower
*/
std::string
classifyPrimerSlow(std::string* seq)
{
    return classifyPrimer(seq, 5);
}

/*
    same as classifyPrimerSlow, but with an even higher edit distance
    very slow, probably not worth it
*/
std::string
classifyPrimerSuperSlow(std::string* seq)
{
    return classifyPrimer(seq, 8);
}