#include <iostream>
#include <vector>
#include <map>
#include <optional>

#include "utilities.h"
#include "Reader.h"
#include "Writer.h"
#include "config.h"
#include "stats.h"
#include "colors.h"

/*
    checks whether restrander has been run with a valid number of arguments,
    prints out some info if necessary
*/
void
checkArgumentCount(int argc)
{
    if (argc != 4) {
        std::cout << colors::print("Wrong number of arguments!", colors::warn) << " Use format:\n"
            << "\trestrander [input filename] [output filename] [config filename]\n";
        program::stop();
    }
}

/*
    prints out some header information
*/
void
printHeader(std::string inputFilename, std::string outputFilename, std::string name)
{
    std::cout << colors::print("Restrander initialised.\n", colors::good)
        << "\tInput file  :\t" << inputFilename << "\n"
        << "\tOutput file :\t" << outputFilename << "\n"
        << "\tPipeline    :\t" << name << "\n";
}

nlohmann::json
toJson(stats::Stats stats, char ** argv)
{
    nlohmann::json json;

    // json["inputFile"] = argv[1];
    // json["outputFile"] = argv[2];
    // json["config"] = config::parseConfigFile(argv[3]);
    json["stats"] = stats::toJson(stats);

    return json;
}


int
main(int argc, char ** argv)
{
    // first, check that the right number of arguments have been supplied
    checkArgumentCount(argc);

    // read in the configuration file, if one is given
    auto config = argc == 4 ? config::parseConfig(argv[3]) : config::makeDefaultConfig();

    // open the files, and check their types
    Reader reader (argv[1]);
    Writer writer (argv[2], config.excludeUnknowns);

    // print out some header information
    if (!config.silent) {
        printHeader(argv[1], argv[2], config.name);
    }

    if (!config.silent) {
        std::cout << colors::print("Started restranding...\n", colors::good);
    }
    
    // initialise stats and record
    stats::Stats stats = {0, {{}}, {{}}};
    Record record = Record();
    int recordNum = 0;

    while (true) {
        // try reading a record
        try {
            record = reader.read();
            record.classify(config.pipeline);
        }
        // or, if we're out of records, break the loop
        catch (const std::invalid_argument &e) {
            break;
        }

        // keep track of the record we're up to
        recordNum += 1;
        if (!config.silent && recordNum % 100000 == 0 && recordNum > 0) {
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

    if (!config.silent) {
        std::cout << colors::print("Finished restranding!\n", colors::good);
    }

    std::cout << toJson(stats, argv).dump(4) << "\n";
    // close the files
    reader.close();
    writer.close();
}