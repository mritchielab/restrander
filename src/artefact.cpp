#include <iostream>
#include <string>
#include <unordered_map>

#include "artefact.h"
#include "utilities.h"

namespace artefact 
{
    /*
        determines whether a read is a TSO-TSO artefact or a VNP-VNP artefact.
    */
    Artefact
    classifyArtefact(
        std::string& seq, int editDistance,
        std::string& tso, std::string& vnp,
        bool TSO, bool VNP
    )
    {
        // look for the reverse complement primers
        bool TSOrevComp = hasTSO(seq, editDistance, reverseComplement(tso), 2000);
        bool VNPrevComp = hasVNP(seq, editDistance, reverseComplement(vnp), 2000);

        // classify based on this
        if (TSO && TSOrevComp) {
            return tsotso;
        } else if (VNP && VNPrevComp) {
            return vnpvnp;
        } else {
            return none;
        }
    }

    /*
        converts an Artefact to its string meaning
    */
    std::string
    getName(Artefact artefact)
    {
        std::unordered_map<Artefact, std::string>
        names;
        
        names['t'] = "TSO-TSO";
        names['v'] = "VNP-VNP";

        return names[artefact];
    }
}