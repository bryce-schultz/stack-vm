#include <cstdio>

#include "../../vm/include/SimpleVirtualMachine.h"
#include "../../vm/exception/include/SVMException.h"
#include "../../vm/parser/include/SVASMParser.h"
#include "../include/LithiumParser.h"
#include "../visitors/generator/include/GeneratorVisitor.h"

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

	GeneratorVisitor generator(argv[1] + std::string(".svasm"));
}