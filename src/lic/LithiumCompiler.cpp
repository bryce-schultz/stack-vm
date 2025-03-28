//***********************************************
// LithiumCompiler.cpp
//***********************************************

#include "LithiumCompiler.h"
#include "LithiumParser.h"
#include "GeneratorVisitor.h"
#include "Util.h"
#include "Error.h"

bool LithiumCompiler::compile(const std::string &filename)
{
    LithiumParser parser;
	const std::string outputFilename = Util::getFileNameWithoutExtension(filename) + OUTPUT_EXTENSION;
	GeneratorVisitor generator(outputFilename);

	auto root = parser.parse(filename);

	if (!root.isValid())
	{
		return false;
	}

	if (global::hadError)
	{
		delete root.getNode();
		return false;
	}
	
	generator.visitAllChildren(root.getNode());

	// done with the AST, clean it up
	delete root.getNode();

	if (global::hadError)
	{
		return false;
	}

	return true;
}
