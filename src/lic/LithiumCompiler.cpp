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

	Node *root = parser.parse(filename);

	if (!root)
	{
		return false;
	}

	if ( global::hadError)
	{
		delete root;
		return false;
	}
	
	generator.visitAllChildren(root);

	// done with the AST, clean it up
	delete root;

	if (global::hadError)
	{
		return false;
	}

	return true;
}
