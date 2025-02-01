#pragma once

#include <string>

#include "Location.h"

class Error
{
public:
    Error(const std::string &message, const Location &location);

    std::string getMessage() const;

    virtual std::string format() const;
protected:
    std::string message;
    Location location;
};