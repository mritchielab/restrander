#ifndef ARTEFACT_H
#define ARTEFACT_H

#include <string>

/* namespace for anything related to artefacts */
namespace artefact {
    using Artefact = char;
    const Artefact none = ' ';
    const Artefact tsotso = 't';
    const Artefact vnpvnp = 'v';

    Artefact
    classifyArtefact(
        std::string& seq, int editDistance, 
        std::string& tso, std::string& vnp, 
        bool TSO, bool VNP
    );

    std::string
    getName(Artefact artefact);

};

#endif