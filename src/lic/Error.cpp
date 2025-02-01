#include "Error.h"

Error::Error(const std::string &message, const Location &location):
    message(message),
    location(location)
{
}

std::string Error::getMessage() const
{
    return format();
}

std::string Error::format() const
{
    int line = location.getLine();
    int column = location.getColumn();
    const std::string &filename = location.getFilename();

    return "error: " + filename + ":" + std::to_string(line) + ":" + std::to_string(column) + ":\n" + message;
}
