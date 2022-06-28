#include <string>
#include <functional>
#include <unordered_map>
#include <iostream>
#include <fstream>

#include "utilities.h"
#include "classify.h"
#include "json.hpp"

/*
    generates the default pipeline,
    used for standard PCB109
*/
Pipeline
makeDefaultPipeline()
{
    using namespace std::placeholders;
    
    Pipeline
    pipeline = {};
    // here we use bind to partially apply a function
    Method poly = std::bind(classifyPoly, 
        _1, 12, 200
    );
    pipeline.push_back(poly);
    Method primer = std::bind(classifyPrimer, 
        _1, 6, "TTTCTGTTGGTGCTGATATTGCTGGG", "ACTTGCCTGTCGCTCTATCTTC"
    );
    pipeline.push_back(primer);

    return pipeline;
}

/*
    classifies whether a sequence is forward or reverse,
    using a given pipeline
*/
Strand
classifyPipeline(std::string& seq, Pipeline& pipeline)
{
    Strand
    strand = unknownStrand;

    for (const auto & classify : pipeline) {
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
classifyPoly(std::string& seq, int tailLength, int searchSize)
{
    bool polyATail = hasPolyATail(seq, tailLength, searchSize);
    bool polyTTail = hasPolyTTail(seq, tailLength, searchSize);

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
classifyPrimer(std::string& seq, int editDist, std::string ssp, std::string vnp)
{   
    bool SSP = hasSSP(seq, editDist, ssp);
    bool VNP = hasVNP(seq, editDist, vnp);

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