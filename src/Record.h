#ifndef RECORD_H
#define RECORD_H

#include <string>

class Record
{
    private:
        std::string
        identifier;

        std::string
        sequence;

        std::string
        quality;

        bool
        rv;

        bool
        ambiguous;

    public:
        Record
        (
            std::string identifier,
            std::string sequence,
            std::string quality
        );

        std::string
        print();

        bool
        classifyReversed(std::string method="safe");

        bool
        isAmbiguous();

        bool
        isReversed();
};

#endif