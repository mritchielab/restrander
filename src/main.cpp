#include <iostream>

#include "FastqParser.h"
#include "utilities.h"

#define C_MAGENTA   "\x1B[35m"
#define C_DEFAULT   "\x1B[0m"
#define C_GREEN     "\x1B[32m"
#define C_RED       "\x1B[31m"
#define C_YELLOW    "\x1B[33m"

int
main(int argc, char ** argv)
{
    if (argc != 3) {
        std::cout << C_YELLOW << "Wrong number of arguments!" << C_DEFAULT << " Use format:\n"
            << "\trestrander [input filename] [output filename]\n" << C_DEFAULT;
        return 0;
    }

    std::string inFilename (argv[1]);
    std::string outFilename (argv[2]);

    std::cout << "Restrander initialised.\n"
        << "\tInput filename  :\t" << inFilename << "\n"
        << "\tOutput filename :\t" << outFilename << "\n";

    std::cout << C_GREEN << "Started restranding...\n" << C_DEFAULT;
    FastqParser parser (inFilename, outFilename);

    std::cout << C_GREEN << "Finished restranding!\n" << C_DEFAULT;

    Stats stats = parser.stats();

    std::cout
        << "\tTotal reads:   " << stats.reads << "\n"
        << "\tForward reads: " << stats.fwReads << "\n"
        << "\tReverse reads: " << stats.rvReads << "\n"
        << "\tAmbig reads:   " << stats.ambigReads << "\n";
}