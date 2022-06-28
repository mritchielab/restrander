#ifndef READER_H
#define READER_H

#include <string>
#include <unordered_map>
#include <functional>
#include <fstream>
#include <variant>
#include <zlib.h>

#include "Record.h"

// using File = std::variant<std::ifstream, gzFile>;

struct Files
{
    std::ifstream* standard;
    gzFile gz;
};

class Reader;

using Stats = std::unordered_map<char, int>;
using ReadFunc = std::function<Record(Reader&)>;
using ReadLineFunc = std::function<std::string(Reader&)>;
using ReaderCloseFunc = std::function<void(Reader&)>;

class Reader
{
    private:
        std::ifstream file;
        gzFile gzippedFile;

        Stats stats;
        ReadFunc readFunc;
        ReadLineFunc readLineFunc;
        ReaderCloseFunc closeFunc;

        Record
        readFq();

        std::string
        readLine();

        std::string
        readLineStandard();

        std::string
        readLineGzipped();

        void
        closeStandard();

        void
        closeGzipped();

    public:
        Reader(std::string filename);

        Record 
        read();

        Stats
        getStats();

        void
        close();
};

#endif