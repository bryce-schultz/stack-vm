#pragma once
#include <cstdint>
#include <string>
#include <vector>

#include "Program.h"

struct SVASMParserResult
{
	bool success	{ false };
	Program program { };
};

class SVASMParser
{
public:
	SVASMParserResult parse(const std::string &source);
private:
	SVASMParserResult parseInternal(const std::string& source);
	bool isWhitespace(char c) const;
};

