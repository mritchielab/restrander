#include <iostream>
#include <zlib.h>

#include "Writer.h"

Writer::Writer (std::string filename)
{
    // get the file extension
    std::string extension = filename.substr(filename.find_last_of(".") + 1);
    if (extension == "gz") {
        std::cout << "gzipped\n";
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
        this->closeFunc = &Writer::close;
    }
    // extension should now be correct

    // make sure that it is in our list of valid file extensions
    std::unordered_map<std::string, WriteFunc>
    validExtensions;
    validExtensions["fq"] = &Writer::writeFq;
    validExtensions["fastq"] = &Writer::writeFq;
    // validExtensions["bam"] = &Writer::writeModbam;
    this->writeFunc = validExtensions[extension];
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