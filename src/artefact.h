#ifndef ARTEFACT_H
#define ARTEFACT_H

#include <string>

/* namespace for anything related to artefacts */
namespace artefact {
    using Artefact = char;
    const Artefact none = ' ';
    const Artefact tsotso = 't';
    const Artefact rtprtp = 'v';

    Artefact
    classifyArtefact(
        std::string& seq, const double errorRate, 
        std::string& tso, std::string& rtp, 
        bool TSO, bool RTP
    );

    std::string
    getName(Artefact artefact);

};

#endif