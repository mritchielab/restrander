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

        std::string
        direction;

    public:
        Record
        (
            std::string identifier,
            std::string sequence,
            std::string quality
        );

        std::string
        print();

        std::string
        classifyDirection(std::string method="safe");

        std::string
        getDirection();
};

#endif