#include <string>
#include <unordered_map>

#include "ukkonenMatch.h"

/*
    compares two characters, allowing for IUPAC codes in the primer sequence.
    assumes no IUPAC codes in the FASTQ sequence from the read itself!
*/
bool
equalIUPAC(char primer_char, char seq_char) 
{
    switch (seq_char) {
        case 'A': switch (primer_char) {
            case 'A':
            case 'W':
            case 'M':
            case 'R':
            case 'D':
            case 'H':
            case 'V':
            case 'N':
                return true;

            default: 
                return false;
        };

        case 'C': switch (primer_char) {
            case 'C':
            case 'S':
            case 'M':
            case 'Y':
            case 'B':
            case 'H':
            case 'V':
            case 'N':
                return true;
            
            default: 
                return false;
        }

        case 'T': switch (primer_char) {
            case 'T':
            case 'W':
            case 'K':
            case 'Y':
            case 'B':
            case 'D':
            case 'H':
            case 'N':
            case 'U': // uracil considered equivalent to thymine
                return true;
            
            default: return false;
        }

        case 'G': switch (primer_char) {
            case 'G':
            case 'S':
            case 'K':
            case 'R':
            case 'B':
            case 'D':
            case 'V':
            case 'N':
                return true;
            
            default: return false;
        }

        // sequence has an invalid character
        default: return false;
    }
}

/*
    determines whether a fuzzy match of key is present in the first searchSize characters of seq,
    using a simple implementation of Ukkonen
*/
bool
containsMatch(std::string &seq, std::string &key, int searchSize, int threshold)
{
    int m = key.length() - 1;
    int n = std::min((int)seq.length(), searchSize) - 1;

    int C[m];
    for (int i = 0; i < m; ++i) {
        C[i] = i;
    }
    int lact = threshold + 1;

    int firstMatchPos = -1; // keep track of first match
    for (int pos = 0; pos < n + 1; ++pos) {
        int Cp = 0, Cn = 0;
        for (int i = 0; i < lact + 1; ++i) {
            if (equalIUPAC(key[i], seq[pos])) {
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
            if (firstMatchPos == -1 || pos < firstMatchPos) {
                firstMatchPos = pos; // always pick earliest
            }
        } else {
            lact++;
        }
    }

    return firstMatchPos != -1;
}
