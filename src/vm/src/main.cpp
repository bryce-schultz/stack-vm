#include <cstdio>

#include "SimpleVirtualMachine.h"
#include "SVMException.h"
#include "../parser/include/SVASMParser.h"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("usage: %s <source>\n", argv[0]);
		return 1;
	}

	SVASMParser asm_parser;
	SVASMParserResult result = asm_parser.parse(argv[0]);

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