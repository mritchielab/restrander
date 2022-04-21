#ifndef RECORD_H
#define RECORD_H

#include <string>

#include "classifier.h"

class Record
{
    private:
        std::string
        identifier;
        std::string
        sequence;
        std::string
        quality;

        Strand
        strand;

    public:
        Record
        (
            std::string identifier,
            std::string sequence,
            std::string quality,
            std::string method
        );

        std::string
        print();

        Strand
        getStrand();
};

#endif