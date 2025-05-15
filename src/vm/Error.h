#pragma once

#include <string>
#include <iostream>
#include <sstream>

#include "Color.h"

#define error(msg) error_(msg, __FILE__, __LINE__)

void error_(const std::string &msg, const std::string &file, int line);