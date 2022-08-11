#include <string>

char
complement(char c);

std::string
complement(std::string& s);

std::string
reverse(std::string& s);

std::string
reverseComplement(std::string& s);

bool
hasPolyATail(std::string& seq, int polyAThreshold=12, int countThreshold=200);

bool
hasPolyTTail(std::string& seq, int polyTThreshold=12, int countThreshold=200);

bool
hasRT(std::string& seq, int thresholdDist=3, std::string RT="ACTTGCCTGTCGCTCTATCTTC", int searchSize=200);

bool
hasTSO(std::string& seq, int thresholdDist=3, std::string TSO="TTTCTGTTGGTGCTGATATTGCTGGG", int searchSize=200);