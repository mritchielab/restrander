#include <iostream>
#include <string>
#include <unordered_map>

#include "artefact.h"
#include "utilities.h"

namespace artefact 
{
    /*
        determines whether a read is a TSO-TSO artefact or a RT-RT artefact.
    */
    Artefact
    classifyArtefact(
        std::string& seq, int editDistance,
        std::string& tso, std::string& rt,
        bool TSO, bool RT
    )
    {
        // look for the reverse complement primers
        bool TSOrevComp = hasTSO(seq, editDistance, reverseComplement(tso), 2000);
        bool RTrevComp = hasRT(seq, editDistance, reverseComplement(rt), 2000);

        // classify based on this
        if (TSO && TSOrevComp) {
            return tsotso;
        } else if (RT && RTrevComp) {
            return rtrt;
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
        names['v'] = "RT-RT";

        return names[artefact];
    }
}