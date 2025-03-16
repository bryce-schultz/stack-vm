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

	bool push(Program &program, const std::vector<std::string> &tokens, size_t &i);
	bool pop(Program &program, const std::vector<std::string> &tokens, size_t &i);
	bool add(Program &program, const std::vector<std::string> &tokens, size_t &i);
	bool sub(Program &program, const std::vector<std::string> &tokens, size_t &i);
	bool mul(Program &program, const std::vector<std::string> &tokens, size_t &i);
	bool div(Program &program, const std::vector<std::string> &tokens, size_t &i);
	bool mod(Program &program, const std::vector<std::string> &tokens, size_t &i);
	bool neg(Program &program, const std::vector<std::string> &tokens, size_t &i);
	bool pow(Program &program, const std::vector<std::string> &tokens, size_t &i);
	bool fact(Program &program, const std::vector<std::string> &tokens, size_t &i);
	bool nop(Program &program, const std::vector<std::string> &tokens, size_t &i);
	bool print(Program &program, const std::vector<std::string> &tokens, size_t &i);
	bool halt(Program &program, const std::vector<std::string> &tokens, size_t &i);
	bool syscall(Program &program, const std::vector<std::string> &tokens, size_t &i);
private:
	SVASMSymbolTable symbolTable;
};

