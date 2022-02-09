#include <string>

char
complement(char c);

std::string
complement(std::string * s);

std::string
reverse(std::string * s);

std::string
reverseComplement(std::string * s);

bool
hasPolyATail(std::string* seq, int polyAThreshold=12, int countThreshold=200);

bool
hasPolyTTail(std::string* seq, int polyTThreshold=12, int countThreshold=200);

bool
hasVNP(std::string* seq, int thresholdDist=3, std::string VNP="ACTTGCCTGTCGCTCTATCTTC", int searchSize=100);

bool
hasSSP(std::string* seq, int thresholdDist=3, std::string SSP="TTTCTGTTGGTGCTGATATTGCTGGG", int searchSize=100);