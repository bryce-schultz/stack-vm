#include <cstdio>

#include "SimpleVirtualMachine.h"
#include "SVMException.h"
#include "SVASMParser.h"
#include "LithiumParser.h"
#include "GeneratorVisitor.h"

#define DEBUG

#define UNUSED(x) (void)(x)

int main(int argc, char **argv)
{
	UNUSED(argc);
	UNUSED(argv);
	/*if (argc != 2)
	{
		printf("usage: %s <source>\n", argv[0]);
		return 1;
	}*/

	const char *filename = "tests/test.li";

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

		return 1;
	}

#ifdef DEBUG
	GeneratorVisitor generator(filename + std::string(".svasm"));
#else
	GeneratorVisitor generator;
#endif
	generator.visitAllChildren(root);

	SVASMParser asm_parser;
	SVASMParserResult result = asm_parser.parse(generator.getOutput());

	if (!result.success)
	{
		printf("error: failed to parse the asm\n");
		return 1;
	}

	SimpleVirtualMachine vm(128KB);
	vm.enableStackCheck(true);

	try
	{
		vm.load(result.program);
	}
	catch (const SVMException &e)
	{
		printf("error: %s\n", e.what());
		return 1;
	}

	return vm.run() ? 0 : 1;
}