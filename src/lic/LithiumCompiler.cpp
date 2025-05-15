//***********************************************
// LithiumCompiler.cpp
//***********************************************

#include "LithiumCompiler.h"
#include "PrintVisitor.h"
#include "LithiumParser.h"
#include "GeneratorVisitor.h"
#include "Util.h"
#include "Error.h"
#include "Options.h"

using global::options;
using global::hadError;

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

	if (hadError)
	{
		delete root.getNode();
		return false;
	}

	if (options.outputAst)
	{
		PrintVisitor printVisitor;
		printVisitor.visitAllChildren(root.getNode());
		printVisitor.outputToFile(Util::getFileNameWithoutExtension(filename) + ".xml");
	}
	
	generator.visitAllChildren(root.getNode());

	// done with the AST, clean it up
	delete root.getNode();

	if (hadError)
	{
		return false;
	}

	return true;
}
