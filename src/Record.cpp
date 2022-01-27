#include "Record.h"

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
}

/*
    prints out a record with correct formatting
*/
std::string
Record::print()
{
    if (this->rv) {
        this->identifier += " _rev";
        this->sequence = reverseComplement(&this->sequence);
        this->quality = reverse(&this->quality);
    }

    return this->identifier + "\n" + this->sequence + "\n+\n" + this->quality + "\n";
}

/*
    classifies whether this is a forward read or a reversed read
*/
bool
isRecordReversed()
{

}