#include <iostream>
#include <vector>
#include <map>

#include "FastqParser.h"
#include "utilities.h"

#define C_MAGENTA   "\x1B[35m"
#define C_DEFAULT   "\x1B[0m"
#define C_GREEN     "\x1B[32m"
#define C_RED       "\x1B[31m"
#define C_YELLOW    "\x1B[33m"

bool
validArgumentCount(int argc)
{
    if (argc < 3) {
        std::cout << C_YELLOW << "Wrong number of arguments!" << C_DEFAULT << " Use format:\n"
            << "\trestrander [input filename] [output filename] (optional: restranding method)\n" << C_DEFAULT;
        return false;
    }
    return true;
}

bool
validMethod(std::string method) {
    std::map<std::string, std::string>
    methods;

    methods["standard"] = "Checks for PolyA and PolyT tails, and in ambiguous cases looks for SSP or VNP. Works in around 99.6% of cases.";
    methods["slow"]     = "Same as standard, but allows for higher edit distance in SSP/VNP. Will result in higher accuracy, but much slower.";
    methods["superslow"]     = "Even higher edit distance than slow, this method allows very large margin for error in SSP/VNP. Use at your own risk.";
    methods["trimmed"]  = "Only searches for PolyA and PolyT tails. Useful for trimmed reads.";

    if (methods.count(method)) {
        std::cout << "Restranding method set to \"" << method << "\"\n";
        return true;

    } else {
        std::cout << C_YELLOW << "Invalid restranding method \"" << method << "\"!" << C_DEFAULT << " Use one of these options:\n\n";
        for (const auto & [key, val] : methods) {
            std::cout << key << ":\n\t" << val << "\n";
        }
        std::cout << "\n";
    }

    return false;
}

int
main(int argc, char ** argv)
{
    // make sure the right number of arguments were given
    if (!validArgumentCount(argc)) {
        return 0;
    }

    std::string method = "standard"; 
    if (argc > 3 && validMethod(argv[3])) {
        method = argv[3];
    }

    // open the necessary files
    std::string inFilename (argv[1]);
    std::string outFilename (argv[2]);

    std::cout << "Restrander initialised.\n"
        << "\tInput filename  :\t" << inFilename << "\n"
        << "\tOutput filename :\t" << outFilename << "\n"
        << "\tMethod          :\t" << method << "\n";

    // start the restrander
    std::cout << C_GREEN << "Started restranding...\n" << C_DEFAULT;
    FastqParser parser (inFilename, outFilename, method);
    std::cout << C_GREEN << "Finished restranding!\n" << C_DEFAULT;

    // print out the stats
    Stats stats = parser.getStats();
    std::cout
        << "\tTotal reads:   " << stats["reads"] << "\n"
        << "\tForward reads: " << stats["forward"] << "\n"
        << "\tReverse reads: " << stats["reverse"] << "\n"
        << "\tAmbig reads:   " << stats["ambiguous"] << "\n";
}