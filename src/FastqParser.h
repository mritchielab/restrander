#ifndef FASTQPARSER_H
#define FASTQPARSER_H

#include <string>

class FastqParser
{
    private:
        std::string filename;

    public:
        FastqParser(std::string inFilename, std::string outFilename);
};

#endif