#include "FastqParser.h"

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <zlib.h>

#include "Record.h"

FastqParser::FastqParser(std::string inFilename, std::string outFilename, std::string method)
{
    this->stats = {};

    if (inFilename.back() == 'z') {
        std::cout << "Gzipped format detected! Parsing gzipped file...\n";
        this->parseGz(inFilename, outFilename, method);
    } else {
        parse(inFilename, outFilename, method);
    }
}

void
FastqParser::parse(std::string inFilename, std::string outFilename, std::string method)
{
    
    std::ifstream
    inFile (inFilename);
    std::ofstream
    outFile (outFilename);

    std::string identifier, sequence, quality;
    int fileLineNum = 0;
    std::string line;

    // double shortestSeq=1000000, longestSeq=0, averageSeq=0;

    // iterate over each line in the file
    while(getline(inFile, line)) {
        if (fileLineNum % 100000 == 0 && fileLineNum > 0) {
            std::cout << "\tUp to line " << fileLineNum << "...\n";
        }
        // work out where we are up to within this record
        int recLineNum = fileLineNum % 4;

        // store whatever is appropriate for this line
        if (recLineNum == 0) {
            identifier = line;
        } else if (recLineNum == 1) {
            sequence = line;
        } else if (recLineNum == 3) {
            quality = line;
            
            // we've got everything about this record
            // time to create it
            Record rec (identifier, sequence, quality, method);
            outFile << rec.print();

            //  keep track of stats as we go
            auto direction = rec.getStrand();
            if (!this->stats.count(direction)) {
                this->stats[direction] = 0;
            }
            this->stats[direction]++;
            this->stats['r']++;

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

void
FastqParser::parseGz(std::string inFilenameGz, std::string outFilenameGz, std::string method)
{
    // open the files
    gzFile
    inFileGz = gzopen(inFilenameGz.c_str(), "r"),
    outFileGz = gzopen(outFilenameGz.c_str(), "w");

    std::string identifier, sequence, quality;
    int fileLineNum = 0;
    char line [500000];

    while (gzgets(inFileGz, line, 500000)) {
        if (fileLineNum % 100000 == 0 && fileLineNum > 0) {
            std::cout << "\tUp to line " << fileLineNum << "...\n";
        }

        //  work out where we are up to in this record
        int recLineNum = fileLineNum % 4;

        //  store whatever is appropriate for this line
        if (recLineNum == 0) {
            identifier = line;
            identifier.pop_back();
        } else if (recLineNum == 1) {
            sequence = line;
            sequence.pop_back();
        } else if (recLineNum == 3) {
            quality = line;
            quality.pop_back();

            //  we've got everything about this record
            //  time to create it
            Record rec (identifier, sequence, quality, method);
            std::string
            outLine = rec.print();
            gzwrite(outFileGz, outLine.c_str(), sizeof(char) * outLine.size());
            
            auto direction = rec.getStrand();
            if (!this->stats.count(direction)) {
                this->stats[direction] = 0;
            }
            this->stats[direction]++;
            this->stats['r']++;
        }
        fileLineNum++;
    }

    gzclose(inFileGz);
    gzclose(outFileGz);
}


/*
    extract the read stats from this parser
*/
Stats
FastqParser::getStats()
{
    return this->stats;
}