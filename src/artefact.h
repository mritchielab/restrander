#ifndef ARTEFACT_H
#define ARTEFACT_H

#include <string>

/* namespace for anything related to artefacts */
namespace artefact {
    using Artefact = char;
    const Artefact none = ' ';
    const Artefact tsotso = 't';
    const Artefact rtrt = 'v';

    Artefact
    classifyArtefact(
        std::string& seq, int editDistance, 
        std::string& tso, std::string& rt, 
        bool TSO, bool RT
    );

    std::string
    getName(Artefact artefact);

};

#endif