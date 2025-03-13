#include "LithiumCompiler.h"
#include "LithiumParser.h"
#include "GeneratorVisitor.h"
#include "Util.h"

bool LithiumCompiler::compile(const std::string &filename)
{
    LithiumParser li_parser;

	Node *root = li_parser.parse(filename);

	if (root == nullptr)
	{
		const auto &errors = li_parser.getErrors();
		for (const auto &error : errors)
		{
			printf("%s\n", error.c_str());
		}

		if (errors.empty())
		{
			printf("error: failed to parse the source\n");
		}

		return false;
	}

	std::string outputFilename = Util::getFileNameWithoutExtension(filename) + outputExtension;

	GeneratorVisitor generator(outputFilename);
	generator.visitAllChildren(root);

	delete root;

	if (generator.hasError())
	{
		return false;
	}

	return true;
}
