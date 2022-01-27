#include "Record.h"

#include <iostream>

#include "utilities.h"

/*
    Initialises a record
*/
Record::Record
(
    std::string identifier,
    std::string sequence,
    std::string quality
)
{
    this->identifier = identifier;
    this->sequence = sequence;
    this->quality = quality;

    this->ambiguous = false;
    this->rev = classifyReverse();

}

/*
    prints out a record with correct formatting
*/
std::string
Record::print()
{
    this->identifier += " direction=";
    
    if (this->ambiguous) {
        this->identifier += "ambiguous";
    } else {
        if (this->rev) {
            this->identifier += "reverse";
            this->sequence = reverseComplement(&this->sequence);
            this->quality = reverse(&this->quality);
        } else {
            this->identifier += "forward";
        }
    }

    return this->identifier + "\n" + this->sequence + "\n+\n" + this->quality + "\n";
}

/*
    classifies whether this is a forward read or a reverse read
*/
bool
Record::classifyReverse(std::string method)
{
    if (method == "fast") {
        return !hasPolyATail(&this->sequence);
    } else if (method == "safe") {
        bool polyATail = hasPolyATail(&this->sequence);
        bool polyTTail = hasPolyTTail(&this->sequence);

        if (polyATail && polyTTail) {
            // std::cout << "Warning: Ambiguous sequence has both tails!\n";
            this->ambiguous = true;
        }

        return polyTTail && !polyATail;
    }

    // should never get out here
    return 0;
}

/*
    checks whether the record is ambiguous
*/
bool
Record::isAmbiguous()
{
    return this->ambiguous;
}

/*
    checks whether the record is reversed
*/
bool
Record::isReverse()
{
    return this->rev;
}