#include <cstdio>

#include "../include/LithiumParser.h"
#include "../visitors/generator/include/GeneratorVisitor.h"

constexpr const char* const extension = ".lsm";

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("usage: %s <source>\n", argv[0]);
		return 1;
	}

	LithiumParser li_parser;

	Node *root = li_parser.parse(argv[1]);

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

		return 1;
	}

	GeneratorVisitor generator(std::string(argv[1]) + extension);
	generator.visitAllChildren(root);

	return 0;
}