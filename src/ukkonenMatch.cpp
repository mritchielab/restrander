#include <string>
#include <iostream>

#include "ukkonenMatch.h"

/*
    determines whether a fuzzy match of key is present in the first searchSize characters of seq,
    using a simple implementation of Ukkonen
*/
bool
containsMatch(std::string &seq, std::string &key, int searchSize, int threshold)
{
    // preprocessing
    int m = key.length() - 1;
    int n = std::min((int)seq.length(), searchSize) - 1;

    int C[m];
    for (int i = 0; i < m; ++i) {
        C[i] = i;
    }
    int lact = threshold + 1; // last active

    // searching
    for (int pos = 0; pos < n + 1; ++pos) {
        int Cp = 0, Cn = 0;
        for (int i = 0; i < lact + 1; ++i) {
            if (key[i] == seq[pos]) {
                Cn = Cp;
            } else {
                if (Cp < Cn) {
                    Cn = Cp;
                }
                if (C[i] < Cn) {
                    Cn = C[i];
                }
                Cn++;
            }
            Cp = C[i];
            C[i] = Cn;
        }

        // updating lact
        while (C[lact] > threshold) {
            lact--;
        }
        if (lact == m) {
            return 1; 
        } else {
            lact++;
        }
    }

    return 0;
}