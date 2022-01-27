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
        isRecordReversed();
};

#endif