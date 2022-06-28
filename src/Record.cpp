#include "Record.h"

#include <iostream>

#include "utilities.h"
#include "classify.h"
#include "json.hpp"

/*
    initialises an empty record
*/
Record::Record()
{

}

/*
    initialises a record
*/
Record::Record
(
    std::string identifier,
    std::string sequence,
    std::string quality
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
}

/*
    classifies the record using a given pipeline
*/
void
Record::classify(Pipeline& pipeline)
{
    this->strand = classifyPipeline(this->sequence, pipeline);
}

/*
    prints out a record with correct formatting
*/
std::string
Record::printFq()
{
    // first, add on the strand tag
    this->identifier += " strand=";
    this->identifier.push_back(this->strand);
    
    // if it's a reverse read, we need to take the reverse of some things
    if (this->strand == '-') {
        this->sequence = reverseComplement(this->sequence);
        this->quality = reverse(this->quality);
    }
    
    // then construct the formatted record
    return this->identifier + "\n" + this->sequence + "\n+\n" + this->quality + "\n";
}