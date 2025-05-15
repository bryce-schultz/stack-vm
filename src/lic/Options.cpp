#include <iostream>
#include <cstring>
#include <string>

#include "Options.h"

namespace global
{
    // global options definition
    Options options;
}

void parseOptions(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0)
        {
            global::options.verbose = true;
        }
        else if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--output-ast") == 0)
        {
            global::options.outputAst = true;
        }
        else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0)
        {
            if (i + 1 < argc)
            {
                global::options.outputFile = argv[++i];
            }
            else
            {
                std::cerr << "error: missing output file name" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
    }

    if (argc > 1)
    {
        global::options.inputFile = argv[argc - 1];
    }
    else
    {
        global::options.inputFile = "";
    }
}