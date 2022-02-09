#include "FastqParser.h"

#include <fstream>
#include <iostream>
#include <unordered_map>

#include "Record.h"

FastqParser::FastqParser(std::string inFilename, std::string outFilename, std::string method)
{
    // open the files
    std::ifstream
    inFile (inFilename);
    std::ofstream
    outFile (outFilename);

    std::string identifier, sequence, quality;
    int fileLineNum = 0;
    std::string line;

    Stats stats;

    // int longestSeq, shortestSeq, averageSeq;

    // iterate over each line in the file
    while(getline(inFile, line)) {
        if (fileLineNum % 100000 == 0 && fileLineNum > 0) {
            std::cout << "\tUp to line " << fileLineNum << "...\n";
        }
        // work out where we are up to within this record
        int rec_line_num = fileLineNum % 4;

        // store whatever is appropriate for this line
        if (rec_line_num == 0) {
            identifier = line;
        } else if (rec_line_num == 1) {
            sequence = line;
        } else if (rec_line_num == 3) {
            quality = line;
            
            // we've got everything about this record
            // time to create it
            Record rec (identifier, sequence, quality, method);
            outFile << rec.print();

            //  keep track of stats as we go
            auto direction = rec.getDirection();
            if (!this->stats.count(direction)) {
                this->stats[direction] = 0;
            }
            this->stats[direction]++;
            this->stats["reads"]++;

            // int seqLength = sequence.size();
            // if (seqLength < shortestSeq) {
            //     shortestSeq = seqLength;
            // } else if (seqLength > longestSeq) {
            //     longestSeq = seqLength;
            // }
            // averageSeq += seqLength;
        }
        fileLineNum++;
    }
    // averageSeq /= this->stats["reads"];
    // std::cout 
    //     << "Shortest seq: " << shortestSeq << "\n"
    //     << "Longest seq: " << longestSeq << "\n"
    //     << "Average seq: " << averageSeq << "\n";
}

/*
    extract the read stats from this parser
*/
Stats
FastqParser::getStats()
{
    return this->stats;
}