#include <iostream>
#include <zlib.h>
#include <filesystem>

#include "Writer.h"
#include "program.h"

Writer::Writer (std::string filename, bool excludeUnknowns)
{
    try {
        // check if this will be the file to print unknowns into
        this->excludeUnknowns = excludeUnknowns;
        if (excludeUnknowns) {
            auto unknownFilename = makeUnknown(filename);
            unknowns = std::make_unique<Writer>(unknownFilename, false);
        }

        // check if the file points to a valid location
        if (!std::filesystem::exists(std::filesystem::path(filename)))
            program::error("Error when creating output file!\nCheck that the path you entered corresponds to a directory that exists.\n");

        // get the file extension
        std::string extension = filename.substr(filename.find_last_of(".") + 1);
        program::note("found extension " + extension + "\n");
        if (extension == "gz") {
            // file is gzipped, record this and slice again
            this->gzippedFile = gzopen(filename.c_str(), "w");
            this->writeLineFunc = &Writer::writeLineGzipped;
            this->closeFunc = &Writer::closeGzipped;
            auto filenameWithoutGz = filename.substr(0,filename.find_last_of("."));
            extension = filenameWithoutGz.substr(filenameWithoutGz.find_last_of(".") + 1);
        } else {
            // file is not gzipped, just open it
            this->file.open(filename);
            this->writeLineFunc = &Writer::writeLineStandard;
            this->closeFunc = &Writer::closeStandard;
        }
        // extension should now be correct

        std::unordered_map<std::string, WriteFunc>
        validExtensions;
        validExtensions["fq"] = &Writer::writeFq;
        validExtensions["fastq"] = &Writer::writeFq;

        // make sure that it is in our list of valid file extensions
        if (validExtensions[extension] == NULL) {
            program::error("Error when creating output file!\nCheck that the path you entered was a fastq file (or gzipped fastq file), not a directory.\n");
        } else {
            this->writeFunc = validExtensions[extension];
        }
    } catch (...) {
        program::error("Error when creating output file!\nCheck that the path you entered was valid.\n");
    }
}

std::string
Writer::makeUnknown(std::string &filename)
{
    auto postfix = "-unknowns";

    // handle filenames without extensions
    if (filename.find_first_of(".") > filename.size())
        return filename + postfix;

    return filename.substr(0, filename.find_first_of(".")) + postfix + filename.substr(filename.find_first_of("."));
}

void
Writer::writeLine(std::string& line)
{
    (this->writeLineFunc)(*this, line);
}

void
Writer::writeLineStandard (std::string& line)
{
    this->file << line;
}

void
Writer::writeLineGzipped(std::string& line)
{
    gzwrite(this->gzippedFile, line.c_str(), sizeof(char) * line.size());
}

void
Writer::write (Record& record)
{
    if (excludeUnknowns && record.strand == strand::unknown)
        unknowns->write(record);
    else
        (this->writeFunc)(*this, record);
}


void
Writer::writeFq (Record& record)
{
    auto 
    line = record.printFq();
    writeLine(line);
}

void
Writer::closeStandard()
{
    file.close();
}

void
Writer::closeGzipped()
{
    gzclose(gzippedFile);
}

void
Writer::close()
{
    (this->closeFunc)(*this);
}