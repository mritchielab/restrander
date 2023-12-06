#include <iostream>
#include <vector>
#include <map>
#include <optional>
#include <string>

#include "utilities.h"
#include "Reader.h"
#include "Writer.h"
#include "config.h"
#include "stats.h"
#include "colors.h"
#include "program.h"

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
meta(std::string inputFilename, bool silent);


int
main(int argc, char ** argv)
{
    // check if we're just searching for help
    program::checkHelp(argc, argv);

    if (program::checkMeta(argc, argv)) {
        meta(argv[1], false);
        // note: THIS FINISHES PROGRAM EXECUTION
    }

    // first, check that the right number of arguments have been supplied
    program::checkArgumentCount(argc);


    // read in the configuration file, if one is given
    auto config = argc == 4 ? config::parseConfig(argv[3]) : config::makeDefaultConfig();

    // open the files, and check their types
    Reader reader (argv[1]);
    Writer writer (argv[2], config.excludeUnknowns);

    // print out some header information
    if (!config.silent) {
        program::printHeader(argv[1], argv[2], config.name);
    }

    if (!config.silent) {
        program::good("Started restranding...\n");
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
            program::good("\tUp to record " + std::to_string(recordNum) + "...\n");
        }

        // write down the record
        writer.write(record);

        // update the stats
        stats.total++;
        stats.strand.stats[record.strand]++;
        stats.artefact.stats[record.artefact]++;
    }

    if (!config.silent) {
        program::good("Finished restranding!\n");
    }

    std::cout << toJson(stats, argv).dump(4) << "\n";
    // close the files
    reader.close();
    writer.close();
}

/*
    run the program in meta-mode, simply quantifying what proportion of unknown reads possess particular qualities
*/
int
meta(std::string inputFilename, bool silent)
{
    // open the files, and check their types
    Reader reader (inputFilename);

    // print out some header information
    if (!silent) {
        program::good("Meta-analysing.\nInput:");
        program::good(inputFilename);
        program::good("\n");
    }

    if (!silent) {
        program::good("Started restranding...\n");
    }
    
    // initialise stats and record
    Record record = Record();
    std::unordered_map<std::string, int>
    stats = {};
    // int nonePolyNonePrimer = 0;
    // int bothPolyNonePrimer = 0;
    // int nonePolyBothPrimer = 0;
    // int bothPolyBothPrimer = 0;
    // int nonePolyArtefact   = 0;
    // int bothPolyArtefact   = 0;

    int recordNum = 0;

    while (true) {
        // try reading a record
        try {
            record = reader.read();

            auto result = metaToString(record.metaClassify());
            if (stats.find(result) == stats.end())
                stats[result] = 0;
            stats[result]++;

            // auto nonePoly   = !result.polyA && !result.polyT;
            // auto bothPoly   =  result.polyA &&  result.polyT;
            // auto nonePrimer = !result.tso   && !result.rtp;
            // auto bothPrimer =  result.tso   &&  result.rtp;

            // nonePolyNonePrimer += nonePoly && nonePrimer;
            // bothPolyNonePrimer += bothPoly && nonePrimer;
            // nonePolyBothPrimer += nonePoly && bothPrimer;
            // bothPolyBothPrimer += bothPoly && bothPrimer;
            // nonePolyArtefact   += nonePoly && result.artefact;
            // bothPolyArtefact   += bothPoly && result.artefact;
        }
        // or, if we're out of records, break the loop
        catch (const std::invalid_argument &e) {
            break;
        }

        // keep track of the record we're up to
        recordNum += 1;
        if (!silent && recordNum % 100000 == 0 && recordNum > 0) {
            // print out a message every 100000 records
            program::good("\tUp to record " + std::to_string(recordNum) + "...\n");
        }
    }

    for (const auto &[key, val] : stats) {
        std::cout << key << "\t" << val << "\n";
    }
    // std::cout <<
    //     "-poly-primer\t" << nonePolyNonePrimer << "\n" <<
    //     "+poly-primer\t" << bothPolyNonePrimer << "\n" <<
    //     "-poly+primer\t" << nonePolyBothPrimer << "\n" <<
    //     "+poly+primer\t" << bothPolyBothPrimer << "\n" <<
    //     "-poly artef.\t" << nonePolyArtefact   << "\n" <<
    //     "+poly artef.\t" << bothPolyArtefact   << "\n";

    if (!silent) {
        program::good("Finished restranding!\n");
    }

    // close the files
    reader.close();

    // finish the program
    program::finish();
}
