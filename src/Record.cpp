#include "Record.h"

#include <iostream>

#include "utilities.h"
#include "classify.h"
#include "json.hpp"
#include "artefact.h"
#include "strand.h"

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
    this->artefact = artefact::none;
}

/*
    classifies the record using a given pipeline
*/
void
Record::classify(Pipeline& pipeline)
{
    auto result = classifyPipeline(this->sequence, pipeline);
    this->strand = result.strand;
    this->artefact = result.artefact;
}

/*
    prints out a record with correct formatting
*/
std::string
Record::printFq()
{
    // first, write down if it's an artefact
    if (this->artefact != artefact::none) {
        this->identifier += " artefact=" + artefact::getName(this->artefact) + " ";
    }

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