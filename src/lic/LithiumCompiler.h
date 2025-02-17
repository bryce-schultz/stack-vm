#pragma once

#include "LithiumParser.h"
#include "GeneratorVisitor.h"

constexpr const char* const outputExtension = ".lsm";

class LithiumCompiler
{
public:
    bool compile(const std::string &filename);
private:
    LithiumParser li_parser;
	GeneratorVisitor generator;
};