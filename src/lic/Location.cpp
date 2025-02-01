#include "Location.h"

Location::Location(int line, int column, const std::string &filename):
    line(line),
    column(column),
    filename(filename)
{
}

bool Location::operator==(const Location &other) const
{
    return line == other.line && column == other.column && filename == other.filename;
}

int Location::getLine() const
{
    return line;
}

int Location::getColumn() const
{
    return column;
}

const std::string &Location::getFilename() const
{
    return filename;
}
