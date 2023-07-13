#include <vector>
#include <iostream>

#include "program.h"
#include "colors.h"

namespace program {
    /*
        terminates the program, called whenever a fatal error occurs
    */
    void
    stop()
    {
        exit(EXIT_FAILURE);
    }

    /*
        terminates the program, called whenever an early successful finish occurs
    */
    void
    finish()
    {
        exit(EXIT_SUCCESS);
    }

    /*
        prints to stderror in a given color
    */
    void
    print(std::string text, std::string color) 
    {
        std::cerr << colors::print(text, color);
    }

    /*
        prints to stderr in the "normal" text color
    */
    void
    note(std::string text) 
    {
        print(text, colors::normal);
    }

    /*
        prints to stderr in the "good" text color
    */
    void
    good(std::string text)
    {
        print(text, colors::good);
    }

    /*
        prints to stderr in the "warn" text color
    */
    void
    warn(std::string text)
    {
        print(text, colors::warn);
    }

    /*
        prints a warning to stderr, and then fails the program
    */
    void
    error(std::string text)
    {
        warn(text);
        stop();
    }

    /*
        checks whether restrander has been run with a valid number of arguments,
        prints out some info if necessary
    */
    void
    checkArgumentCount(int argc)
    {
        if (argc != 4) {
            note(colors::print("Wrong number of arguments!", colors::warn) + 
                " Use format:\n" + 
                "\trestrander [input filename] [output filename] [config filename]\n");
            program::stop();
        }
    }

    /*
        checks whether restrander has been run with the -h/--help argument
    */
    void
    checkHelp(int argc, char ** argv)
    {
        std::vector<std::string> help_synonyms = {"-h", "--help"};

        for (int i = 0; i < argc; i++) {
            for (const auto & h : help_synonyms) {
                if (std::string(argv[i]) == h) {
                    note("Restrander (" + program::version + ") is a fast, accurate program for orienting and quality-checking cDNA sequencing reads.\nFor in-depth documentation, read the vignette: " + colors::good + "https://github.com/jakob-schuster/restrander-vignette\n" + colors::normal);
                    finish();
                }
            }
        }
    }

    /*
        prints out some header information
    */
    void
    printHeader(std::string inputFilename, std::string outputFilename, std::string name)
    {
        std::cerr << colors::print("Restrander initialised.\n", colors::good)
            << "\tInput file  :\t" << inputFilename << "\n"
            << "\tOutput file :\t" << outputFilename << "\n"
            << "\tPipeline    :\t" << name << "\n";
    }
}
