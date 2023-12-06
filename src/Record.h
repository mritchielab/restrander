#ifndef RECORD_H
#define RECORD_H

#include <string>

#include "classify.h"
#include "artefact.h"
#include "json.hpp"

class Record
{
    private:
        std::string
        identifier;
        std::string 
        sequence;
        std::string
        quality;

    public:
        Record();

        Record
        (
            std::string identifier,
            std::string sequence,
            std::string quality
        );

        void
        classify(Pipeline& pipeline);

        std::string
        printFq();

        strand::Strand
        strand;

        artefact::Artefact
        artefact;

        MetaResult
        metaClassify();
};

#endif