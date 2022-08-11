#include <string>
#include <functional>
#include <unordered_map>
#include <iostream>
#include <fstream>

#include "utilities.h"
#include "classify.h"
#include "json.hpp"
#include "artefact.h"
#include "strand.h"
#include "Record.h"

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
        _1, 6, "TTTCTGTTGGTGCTGATATTGCTGGG", "ACTTGCCTGTCGCTCTATCTTC", false
    );
    pipeline.push_back(primer);

    return pipeline;
}

/*
    classifies whether a sequence is forward or reverse,
    using a given pipeline
*/
Result
classifyPipeline(std::string& seq, Pipeline& pipeline)
{
    Result
    result = {strand::unknown, artefact::none};

    for (const auto & classify : pipeline) {
        // try this classifier
        result = classify(seq);

        // if we've successfully classified it, end loop
        if (result.strand != strand::unknown && result.artefact == artefact::none) {
            break;
        }
    }

    return result;
}

/*
    classifies a sequence based on the presence of a PolyA or PolyT tail
    works around 99% of the time
*/
Result
classifyPoly(std::string& seq, int tailLength, int searchSize)
{
    strand::Strand
    strand = strand::unknown;

    bool polyATail = hasPolyATail(seq, tailLength, searchSize);
    bool polyTTail = hasPolyTTail(seq, tailLength, searchSize);

    if (polyATail && !polyTTail) {
        strand = strand::forward;
    } else if (!polyATail && polyTTail) {
        strand = strand::reverse;
    } else if ((polyATail && polyTTail) || (!polyATail && !polyTTail)) {
        strand = strand::unknown;
    }

    // should never get here
    return {strand, artefact::none};
}

/*
    classifies a sequence by looking for the TSO or RT near the start,
    using a given an edit distance
*/
Result
classifyPrimer(
    std::string& seq, int editDist, 
    std::string tso, std::string rt, 
    bool reportArtefacts
)
{   
    strand::Strand
    strand = strand::unknown;

    bool TSO = hasTSO(seq, editDist, tso);
    bool RT = hasRT(seq, editDist, rt);

    // do artefact detection if necessary
    artefact::Artefact
    artefact = reportArtefacts ? 
        artefact::classifyArtefact(seq, editDist, tso, rt, TSO, RT) : 
        artefact::none;

    if (TSO && !RT) {
        strand = strand::forward;
    } else if (!TSO && RT) {
        strand = strand::reverse;
    } else if ((!TSO && !RT) || (TSO && RT)) {
        strand = strand::unknown;
    }

    // should never get here
    return {strand, artefact};
}