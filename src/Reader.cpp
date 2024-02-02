#include <string>
#include <fstream>
#include <iostream>

#include "Reader.h"
#include "Record.h"
#include "colors.h"
#include "utilities.h"
#include "program.h"

/*
    sets up a file reader, working out the necessary parsing method based on the file extension
*/
Reader::Reader (std::string filename)
{
    try {
        // get the file extension
        std::string
        extension = filename.substr(filename.find_last_of(".") + 1);
        if (extension == "gz") {
            // file is gzipped, record this and slice again
            this->gzippedFile = gzopen(filename.c_str(), "r");
            this->readLineFunc = &Reader::readLineGzipped;
            this->closeFunc = &Reader::closeGzipped;
            auto filenameWithoutGz = filename.substr(0, filename.find_last_of("."));
            extension = filenameWithoutGz.substr(filenameWithoutGz.find_last_of(".") + 1);
        } else {
            // file is not gzipped, just open it
            this->file.open(filename);
            this->readLineFunc = &Reader::readLineStandard;
            this->closeFunc = &Reader::closeStandard;
        }
        // extension should now be correct

        // make sure that it is in our list of valid file extensions
        // (this is just for when we add support for other formats in the future)
        std::unordered_map<std::string, ReadFunc>
        validExtensions;
        validExtensions["fq"] = &Reader::readFq;
        validExtensions["fastq"] = &Reader::readFq;
        // validExtensions["bam"] = &Reader::readBam;

        this->readFunc = validExtensions[extension];
    } catch (...) {
        program::error("Error when opening input file!\nCheck that the path you entered was valid.\n");
    }
}

/*
    read a line from the file,
    using an appropriate method for reading lines
*/
std::string
Reader::readLine()
{
    return (this->readLineFunc)(*this);
}

/*
    read a line from a standard, non-zipped file
*/
std::string
Reader::readLineStandard()
{
    std::string line;

    if (!getline(this->file, line)) {
        throw std::invalid_argument ("No more lines to read!");
    }
    return line;
}

#define STATIC_GZIPPED_LINE_LENGTH 50000000
char STATIC_GZIPPED_LINE [STATIC_GZIPPED_LINE_LENGTH];

/*
    read a line from a gzipped file
*/
std::string
Reader::readLineGzipped()
{
    if (!gzgets(this->gzippedFile, STATIC_GZIPPED_LINE, STATIC_GZIPPED_LINE_LENGTH)) {
        throw std::invalid_argument ("No more lines to read!");
    }
    // remove the last character, as it's a newline
    STATIC_GZIPPED_LINE[strlen(STATIC_GZIPPED_LINE)-1]='\0';
    return STATIC_GZIPPED_LINE;
}

/*
    reads one record from the file,
    calling the specialised function for the format
*/
Record
Reader::read()
{
    return (this->readFunc)(*this);
}

/*
    reads one record of a fastq file
*/
Record
Reader::readFq()
{
    auto head = readLine();
    auto seq = readLine();
    auto plus = readLine();
    auto qual = readLine();

    Record record = {head, seq, qual};

    return record;
}

void
Reader::closeStandard()
{
    file.close();
}

void
Reader::closeGzipped()
{
    gzclose(gzippedFile);
}

void
Reader::close()
{
    (closeFunc)(*this);
}