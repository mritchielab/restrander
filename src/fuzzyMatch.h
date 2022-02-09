#include <string>

bool
isFuzzyMatchPresent(std::string* seq, std::string* key, int searchSize, int thresholdDist=3);

int
findFirstFuzzyMatch(std::string* seq, std::string* key, int searchSize=200, int thresholdDist=3);

bool
isFuzzyMatch(std::string* seq, int seqPos, std::string* key, int keyPos, int currentDist=0, int thresholdDist=3);
