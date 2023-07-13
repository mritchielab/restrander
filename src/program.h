#ifndef PROGRAM_H
#define PROGRAM_H

namespace program {
    const std::string version = "v1.0.1";

    void
    note(std::string text);

    void
    good(std::string text);

    void
    warn(std::string text);

    void
    error(std::string text);

    void
    checkArgumentCount(int argc);

    void
    checkHelp(int argc, char ** argv);

    void
    printHeader(std::string inputFilename, std::string outputFilename, std::string name);
}

#endif