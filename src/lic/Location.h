#pragma once

#include <string>

class Location
{
public:
    Location(int line, int column, const std::string &filename);

    bool operator==(const Location &other) const;

    int getLine() const;
    int getColumn() const;
    const std::string &getFilename() const;

private:
    int line;
    int column;
    std::string filename;
};