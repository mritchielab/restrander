#include <string>
#include <functional>
#include <unordered_map>

#include "utilities.h"
#include "classifier.h"

/*
    classifies whether a sequence is forward or reverse,
    using a given method
*/
Strand
classifyStrand(std::string* seq, std::string method)
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
Strand
classifyPipeline(std::string* seq, Pipeline funcs)
{
    Strand
    strand = unknownStrand;
    
    for (const auto & classify : funcs) {
        // try this classifier
        strand = classify(seq);

        // if we've successfully classified it, end loop
        if (strand != unknownStrand) {
            break;
        }
    }

    return strand;
}

/*
    classifies a sequence based on the presence of a PolyA or PolyT tail
    works around 99% of the time
*/
Strand
classifyPoly(std::string* seq)
{
    bool polyATail = hasPolyATail(seq);
    bool polyTTail = hasPolyTTail(seq);

    if (polyATail && !polyTTail) {
        return forwardStrand;
    } else if (!polyATail && polyTTail) {
        return reverseStrand;
    } else if ((polyATail && polyTTail) || (!polyATail && !polyTTail)) {
        return unknownStrand;
    }

    // should never get here
    return unknownStrand;
}

/*
    classifies a sequence by looking for the SSP or VNP near the start,
    using a given an edit distance
*/
Strand
classifyPrimer(std::string* seq, int editDist)
{   
    bool SSP = hasSSP(seq, editDist);
    bool VNP = hasVNP(seq, editDist);

    if (SSP && !VNP) {
        return forwardStrand;
    } else if (!SSP && VNP) {
        return reverseStrand;
    } else if ((!SSP && !VNP) || (SSP && VNP)) {
        return unknownStrand;
    }

    // should never get here
    return unknownStrand;
}

/*
    runs classifyPrimer with a reasonable edit distance,
    runs fairly quickly
*/
Strand
classifyPrimerFast(std::string* seq)
{
    return classifyPrimer(seq, 3);
}

/*
    same as classifyPrimer, but with an increased edit distance
    more accurate but slower
*/
Strand
classifyPrimerSlow(std::string* seq)
{
    return classifyPrimer(seq, 5);
}

/*
    same as classifyPrimerSlow, but with an even higher edit distance
    very slow, probably not worth it
*/
Strand
classifyPrimerSuperSlow(std::string* seq)
{
    return classifyPrimer(seq, 7);
}