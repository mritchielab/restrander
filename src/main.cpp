#include <iostream>
#include <vector>
#include <map>

#include "utilities.h"
#include "Reader.h"
#include "Writer.h"
#include "config.h"
#include "stats.h"

#define C_MAGENTA   "\x1B[35m"
#define C_DEFAULT   "\x1B[0m"
#define C_GREEN     "\x1B[32m"
#define C_RED       "\x1B[31m"
#define C_YELLOW    "\x1B[33m"

/*
    checks whether restrander has been run with a valid number of arguments,
    prints out some info if necessary
*/
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

/*
    prints out some header information
*/
void
printHeader(std::string inputFilename, std::string outputFilename, std::string name)
{
    std::cout << C_GREEN << "Restrander initialised.\n" << C_DEFAULT
        << "\tInput file  :\t" << inputFilename << "\n"
        << "\tOutput file :\t" << outputFilename << "\n"
        << "\tPipeline    :\t" << name << "\n";
}


int
main(int argc, char ** argv)
{
    // first, check that the right number of arguments have been supplied
    if (!validArgumentCount(argc)) {
        return 1;
    }

    std::string
    configFilename = argv[3];

    // then, check if the config specifies running in "silent" mode
    bool silent = false;
    if (argc == 4) {
        silent = config::isSilent(configFilename);
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
        printHeader(argv[1], argv[2], name);
    }

    if (!silent) {
        std::cout << C_GREEN << "Started restranding...\n" << C_DEFAULT;
    }
    
    // initialise stats and record
    stats::Stats stats = {0, {{}}, {{}}};
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
        stats.total++;
        stats.strand.stats[record.strand]++;
        stats.artefact.stats[record.artefact]++;
    }

    if (!silent) {
        std::cout << C_GREEN << "Finished restranding!\n" << C_DEFAULT;
    }

    // print out the stats
    std::cout
        << "\tTotal reads :\t" << stats.total << "\n"
        << "\t+ reads     :\t" << stats.strand.stats[strand::forward] << "\n"
        << "\t- reads     :\t" << stats.strand.stats[strand::reverse] << "\n"
        << "\t? reads     :\t" << stats.strand.stats[strand::unknown] << "\n"
        << "\tt reads     :\t" << stats.artefact.stats[artefact::tsotso] << "\n"
        << "\tv reads     :\t" << stats.artefact.stats[artefact::rtprtp] << "\n";
    
    // close the files
    reader.close();
    writer.close();
}