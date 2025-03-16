#pragma once

#include <string>

class Location
{
public:
    Location();
    Location(size_t line, size_t column, const std::string &filename);

    bool operator==(const Location &other) const;

    size_t getLine() const;
    size_t getColumn() const;
    const std::string &getFilename() const;

    void setLine(size_t line);
    void setColumn(size_t column);
    void setFilename(const std::string &filename);

    void incrementLine(size_t count = 1);
    void incrementColumn(size_t count = 1);
private:
    size_t line;
    size_t column;
    std::string filename;
};