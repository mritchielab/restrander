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
        std::string& seq, const double errorRate,
        std::string& tso, std::string& rtp,
        bool TSO, bool RTP
    )
    {
        // look for the reverse complement primers
        // (for now, be lazy - only look for the reverse primer that will prove it's an artefact)
        bool TSOrevComp = TSO ? hasTSO(seq, getEditDist(errorRate, tso), reverseComplement(tso), 2000) : false;
        bool RTPrevComp = RTP ? hasRTP(seq, getEditDist(errorRate, rtp), reverseComplement(rtp), 2000) : false;

        // classify based on this
        if (TSO && TSOrevComp) {
            return tsotso;
        } else if (RTP && RTPrevComp) {
            return rtprtp;
        } else {
            return none;
        }
    }

    /*
        converts an Artefact to its string name
    */
    std::string
    getName(Artefact artefact)
    {
        std::unordered_map<Artefact, std::string>
        names;
        
        names[tsotso] = "TSO-TSO";
        names[rtprtp] = "RTP-RTP";
        names[none]   = "no artefact";

        return names[artefact];
    }
}