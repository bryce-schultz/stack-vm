#include <cstdio>

#include "SimpleVirtualMachine.h"
#include "SVMException.h"
#include "SVASMParser.h"
#include "LithiumParser.h"
#include "GeneratorVisitor.h"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("usage: %s <source>\n", argv[0]);
		return 1;
	}

	char *filename = argv[1];

	LithiumParser li_parser;

	Node *root = li_parser.parse(filename);

	if (root == nullptr)
	{
		const auto& errors = li_parser.getErrors();
		for (const auto& error : errors)
		{
			printf("%s\n", error.c_str());
		}

		if (errors.empty())
		{
			printf("error: failed to parse the source\n");
		}

		return 1;
	}

	GeneratorVisitor generator(filename + std::string("_out.svasm"));
	generator.visitAllChildren(root);

	SVASMParser asm_parser;
	SVASMParserResult result = asm_parser.parse(filename + std::string("_out.svasm"));

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
	catch (const SVMException& e)
	{
		printf("error: %s\n", e.what());
	}

	return vm.run() ? 0 : 1;
}