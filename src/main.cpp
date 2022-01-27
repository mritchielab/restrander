#include <iostream>

#include "FastqParser.h"
#include "utilities.h"

int
main()
{
    // FastqParser parser ("../data/long/barcode01.pass1.fq", "outfile.fq");
    FastqParser parser ("../data/short/barcode01.pass1.short.fq", "outfile.fq");
    std::string seq = "GAC";
    std::cout << "seq:" << seq;
    std::cout << "complement:" << complement(&seq);
    std::cout << "reverse complement:" << reverseComplement(&seq);
    std::cout << "seq:" << seq;
}