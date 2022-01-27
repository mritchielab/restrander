#include "FastqParser.h"

#include <fstream>
#include <iostream>

#include "Record.h"

FastqParser::FastqParser(std::string inFilename, std::string outFilename)
{
    // open the files
    std::ifstream
    inFile (inFilename);
    std::ofstream
    outFile (outFilename);

    std::string identifier, sequence, quality;
    int file_line_num = 0;
    std::string line;
    // iterate over each line in the file
    while(getline(inFile, line)) {
        if (file_line_num % 100000 == 0) {
            std::cout << "up to line " << file_line_num << "\n";
        }
        // work out where we are up to within this record
        int rec_line_num = file_line_num % 4;

        // store whatever is appropriate for this line
        if (rec_line_num == 0) {
            identifier = line;
        } else if (rec_line_num == 1) {
            sequence = line;
        } else if (rec_line_num == 3) {
            quality = line;
            
            // we've got everything about this record
            // time to create it
            Record rec (identifier, sequence, quality);

            outFile << rec.print();
        }
        file_line_num++;
    }
}