#ifndef STATS_H
#define STATS_H

#include <unordered_map>

#include "strand.h"
#include "artefact.h"

namespace stats {
    /*
        holds stats to do with the stranding of reads
    */
    struct StrandStats
    {
        std::unordered_map<strand::Strand, int> stats;
    };

    /*
        holds stats to do with the artefacts that have been found
    */
    struct ArtefactStats
    {
        std::unordered_map<artefact::Artefact, int> stats;
    };

    /*
        holds all the necessary stats about the reads in the file being parsed
    */
    struct Stats
    {
        int total;
        StrandStats strand;
        ArtefactStats artefact;
    };
}

#endif