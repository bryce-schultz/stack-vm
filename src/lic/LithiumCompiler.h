//***********************************************
// LithiumCompiler.h
//***********************************************

#pragma once

#include "LithiumParser.h"
#include "GeneratorVisitor.h"

constexpr const char* const OUTPUT_EXTENSION = ".lsm";

class LithiumCompiler
{
public:
    bool compile(const std::string &filename);
private:
    LithiumParser parser;
	GeneratorVisitor generator;
};