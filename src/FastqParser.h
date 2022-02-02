#ifndef FASTQPARSER_H
#define FASTQPARSER_H

#include <string>
#include <unordered_map>

typedef std::unordered_map<std::string, int> Stats;

class FastqParser
{
    private:
        std::string filename;
        Stats stats;

    public:
        FastqParser(std::string inFilename, std::string outFilename);

        Stats
        getStats();
};

#endif