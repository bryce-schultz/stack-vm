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

	std::cout << "Parsing complete" << std::endl;

	if (global::options.outputAst)
	{
		PrintVisitor printVisitor;
		printVisitor.visitAllChildren(root.getNode());
		printVisitor.outputToFile(Util::getFileNameWithoutExtension(filename) + ".xml");
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
