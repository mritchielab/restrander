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
    
    this->direction = classifyDirection();
}

/*
    prints out a record with correct formatting
*/
std::string
Record::print()
{
    // first, add on the direction tag
    this->identifier += " direction=" + this->direction;
    
    // if it's a reverse read, we need to take the reverse of some things
    if (this->direction == "reverse") {
        this->sequence = reverseComplement(&this->sequence);
        this->quality = reverse(&this->quality);
    }
    
    // then construct the formatted record
    return this->identifier + "\n" + this->sequence + "\n+\n" + this->quality + "\n";
}

/*
    classifies whether this is a forward read or a reverse read
*/
std::string
Record::classifyDirection(std::string method)
{
    if (method == "fast") {
        if (!hasPolyATail(&this->sequence)) {
            return "forward";
        } else {
            return "reverse";
        }
    } else if (method == "safe") {
        bool polyATail = hasPolyATail(&this->sequence);
        bool polyTTail = hasPolyTTail(&this->sequence);

        if (polyATail && !polyTTail) {
            return "forward";
        } else if (!polyATail && polyTTail) {
            return "reverse";
        } else if ((polyATail && polyTTail) || (!polyATail && !polyTTail)) {
            return "ambiguous";
        }
    }

    // should never get out here
    return "ambiguous";
}

/*
    checks whether the record is ambiguous
*/
std::string
Record::getDirection()
{
    return this->direction;
}