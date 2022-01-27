#ifndef FASTQPARSER_H
#define FASTQPARSER_H

#include <string>

struct Stats
{
    int reads, fwReads, rvReads, ambigReads;
};

class FastqParser
{
    private:
        std::string filename;
        int reads=0, fwReads=0, rvReads=0, ambigReads=0;

    public:
        FastqParser(std::string inFilename, std::string outFilename);

        Stats
        stats();
};

#endif