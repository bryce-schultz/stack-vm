#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>

#include "Program.h"

struct SVASMParserResult
{
	bool success	{ false };
	Program program { };
};

class SVASMSymbolTable
{
public:
	void add(const std::string &name, uint64_t value);
	int64_t get(const std::string &name) const;

	void dump() const;
private:
	std::unordered_map<std::string, uint64_t> symbols;
};

class SVASMParser
{
public:
	SVASMParserResult parse(const std::string &source);
private:
	SVASMParserResult parseInternal(const std::string& source);
	bool isWhitespace(char c) const;
	bool isAddress(const std::string &token) const;

	int64_t getOffset(const std::vector<std::string> &tokens, size_t &i) const;
private:
	SVASMSymbolTable symbolTable;
};

