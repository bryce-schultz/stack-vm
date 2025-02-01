#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <string>

#include "LithiumParser.h"
#include "GeneratorVisitor.h"
#include "Util.h"

constexpr const char* const outputExtension = ".lsm";
constexpr uint8_t requiredArgCount = 2;

bool runCompiler(const std::string &filename);

int main(int argc, char **argv)
{
	if (argc != requiredArgCount)
	{
		printf("usage: %s <source>\n", argv[0]);
		return EXIT_FAILURE;
	}

	if (!runCompiler(argv[1]))
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

bool runCompiler(const std::string &filename)
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

	return true;
}
