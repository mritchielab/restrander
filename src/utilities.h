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
hasPolyATail(std::string * seq, int polyAThreshold=12);

bool
hasPolyTTail(std::string * seq, int polyTThreshold=12);