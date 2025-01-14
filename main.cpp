#include <cstdio>

#include "SimpleVirtualMachine.h"
#include "SVMException.h"
#include "SVASMParser.h"
#include "LithiumParser.h"
#include "GeneratorVisitor.h"

int main()
{
	LithiumParser li_parser;

	Node *root = li_parser.parse("test.li");

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

	GeneratorVisitor generator("test_out.svasm");
	generator.visitAllChildren(root);

	SVASMParser asm_parser;
	SVASMParserResult result = asm_parser.parse("test_out.svasm");

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