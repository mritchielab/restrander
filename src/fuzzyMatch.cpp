#include <string>
#include <iostream>

#include "fuzzyMatch.h"

/*
    determines whether a fuzzy match of key is present in the first searchSize characters of seq
*/
bool
isFuzzyMatchPresent(std::string * seq, std::string key, int searchSize, int thresholdDist)
{
    // first determine the maximum size of the search
    int size = std::min((int)seq->size(), searchSize);

    // see if we find it before this point
    if (findFirstFuzzyMatch(seq, key, searchSize, thresholdDist) < size) {
        return true;
    }

    // otherwise, it wasn't present 
    return false;
}

/*
    searches for a fuzzy match of key in the first searchSize characters of seq
*/
int
findFirstFuzzyMatch(std::string *seq, std::string key, int searchSize, int thresholdDist)
{
    int i;
    int size = std::min((int)seq->size(), searchSize);
    for (i=0; i<searchSize; ++i) {
        if (isFuzzyMatch(seq, i, key, 0, 0, thresholdDist)) {
            break;
        }
    }
    return i;
}

/*
    recursively determines whether seq from seqPos is a fuzzy match of key
*/
bool
isFuzzyMatch(std::string* seq, int seqPos, std::string key, int keyPos, int currentDist, int thresholdDist)
{
    // too many errors, stop the recursion
    if (currentDist > thresholdDist) {
        return false;
    }

    // we've reached the end of the seq, stop the recursion
    if (seqPos == (int)seq->length()) {
        return false;
    }

    // we've reached the end of the key
    if (keyPos == (int)key.length()) {
        return true;
    }

    // the current character matches perfectly
    if ((*seq)[seqPos] == key[keyPos]) {
        return isFuzzyMatch(seq, seqPos+1, key, keyPos+1, currentDist, thresholdDist);
    }

    // the current character does not match, split the timeline
    if ((*seq)[seqPos] != key[keyPos]) {
        return
            // insertion
            isFuzzyMatch(seq, seqPos+1, key, keyPos, currentDist+1, thresholdDist) ||
            // replacement
            isFuzzyMatch(seq, seqPos+1, key, keyPos+1, currentDist+1, thresholdDist) ||
            // deletion
            isFuzzyMatch(seq, seqPos, key, keyPos+1, currentDist+1, thresholdDist);     
    }
  
    // should never get here
    return false;
}