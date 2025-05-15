#pragma once

#include <string>

struct Options
{
    bool verbose = false;
    bool outputAst = false;
    std::string inputFile;
    std::string outputFile;
};

// all globals must be in the global namespace
namespace global
{
    // global options declaration
    extern Options options;
}

void parseOptions(int argc, char **argv);