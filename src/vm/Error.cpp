#include "Error.h"

void error_(const std::string &msg, const std::string &file, int line)
{
    std::cerr << red << "error" << white << ": " << file << ":" << line << "\n│  " << msg << "\n";
}