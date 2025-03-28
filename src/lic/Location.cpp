//***********************************************
// Location.cpp
//***********************************************

#include "Location.h"

Location::Location():
    line(0),
    column(0),
    filename("")
{
}

Location::Location(size_t line, size_t column, const std::string &filename):
    line(line),
    column(column),
    filename(filename)
{
}

bool Location::operator==(const Location &other) const
{
    return line == other.line && column == other.column && filename == other.filename;
}

size_t Location::getLine() const
{
    return line;
}

size_t Location::getColumn() const
{
    return column;
}

void Location::setLine(size_t line)
{
    this->line = line;
}

void Location::setColumn(size_t column)
{
    this->column = column;
}

void Location::setFilename(const std::string &filename)
{
    this->filename = filename;
}

void Location::incrementLine(size_t count)
{
    line += count;
}

void Location::incrementColumn(size_t count)
{
    column += count;
}

const std::string &Location::getFilename() const
{
    return filename;
}
