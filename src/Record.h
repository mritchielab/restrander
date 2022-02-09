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
            std::string quality,
            std::string method
        );

        std::string
        print();

        std::string
        getDirection();
};

#endif