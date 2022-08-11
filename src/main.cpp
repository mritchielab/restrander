#include <iostream>
#include <vector>
#include <map>

#include "utilities.h"
#include "Reader.h"
#include "Writer.h"
#include "config.h"

#define C_MAGENTA   "\x1B[35m"
#define C_DEFAULT   "\x1B[0m"
#define C_GREEN     "\x1B[32m"
#define C_RED       "\x1B[31m"
#define C_YELLOW    "\x1B[33m"

bool
validArgumentCount(int argc)
{
    if (argc == 3) {
        std::cout << C_YELLOW << "No configuration provided, using defaults!\n" << C_DEFAULT;
        return true;
    }

    if (argc != 4) {
        std::cout << C_YELLOW << "Wrong number of arguments!" << C_DEFAULT << " Use format:\n"
            << "\trestrander [input filename] [output filename] (config filename)\n" << C_DEFAULT;
        return false;
    }
    return true;
}

int
main(int argc, char ** argv)
{
    // first, check that the right number of arguments have been supplied
    if (!validArgumentCount(argc)) {
        return 1;
    }

    // then, check if the config specifies running in "silent" mode
    bool silent = false;
    if (argc == 4) {
        silent = config::isSilent(argv[3]);
    }

    // open the files, and check their types
    Reader reader (argv[1]);
    Writer writer (argv[2]);
    // generate the pipeline from config
    Pipeline pipeline = makeDefaultPipeline();
    if (argc == 4) {
        pipeline = config::makePipeline(argv[3]);
    }

    // get the name of the current configuration
    std::string name = "PCB109";
    if (argc == 4) {
        name = config::getName(argv[3]);
    }

    // print out some header information
    if (!silent) {
        std::cout << C_GREEN << "Restrander initialised.\n" << C_DEFAULT
            << "\tInput file  :\t" << argv[1] << "\n"
            << "\tOutput file :\t" << argv[2] << "\n"
            << "\tPipeline    :\t" << name << "\n";
    }

    if (!silent) {
        std::cout << C_GREEN << "Started restranding...\n" << C_DEFAULT;
    }
    
    // initialise stats and record
    Stats stats = {};
    Record record = Record();
    int recordNum = 0;

    while (true) {
        // try reading a record
        try {
            record = reader.read();
            record.classify(pipeline);
        }
        // or, if we're out of records, break the loop
        catch (const std::invalid_argument &e) {
            break;
        }

        // keep track of the record we're up to
        recordNum += 1;
        if (!silent && recordNum % 100000 == 0 && recordNum > 0) {
            // print out a message every 100000 records
            std::cout << "\tUp to record " << recordNum << "...\n";
        }

        // write down the record
        writer.write(record);
        // update the stats
        stats['r']++;
        stats[record.strand]++;
        stats[record.artefact]++;
    }

    if (!silent) {
        std::cout << C_GREEN << "Finished restranding!\n" << C_DEFAULT;
    }

    // print out the stats
    std::cout
        << "\tTotal reads :\t" << stats['r'] << "\n"
        << "\t+ reads     :\t" << stats['+'] << "\n"
        << "\t- reads     :\t" << stats['-'] << "\n"
        << "\t? reads     :\t" << stats['?'] << "\n"
        << "\tt reads     :\t" << stats['t'] << "\n"
        << "\tv reads     :\t" << stats['v'] << "\n";
    
    // close the files
    reader.close();
    writer.close();
}