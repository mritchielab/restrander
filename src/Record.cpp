#include "Record.h"

#include <iostream>

#include "utilities.h"
#include "classifier.h"

/*
    initialises a record
*/
Record::Record
(
    std::string identifier,
    std::string sequence,
    std::string quality,
    std::string method
)
{
    // std::cout << "created record:\n"
    //         << "\tidentifier: " << identifier << "\n"
    //         << "\tsequence: " << sequence << "\n" 
    //         << "\tquality: " << quality << "\n"
            // << "\tmethod: " << method << "\n";

    this->identifier = identifier;
    this->sequence = sequence;
    this->quality = quality;
    
    this->direction = classifyDirection(&this->sequence, method);
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
    checks whether the record is ambiguous
*/
std::string
Record::getDirection()
{
    return this->direction;
}