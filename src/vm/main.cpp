#include <cstdio>
#include <unistd.h>

#include "SimpleVirtualMachine.h"
#include "Error.h"

//#define DEBUG

int main(int argc, char **argv)
{
	std::string filename = "test.lsm";
#ifndef DEBUG
	if (argc != 2)
	{
		printf("usage: %s <source>\n", argv[0]);
		return 1;
	}

	filename = argv[1];
#endif

	SVASMParser asm_parser;
	SVASMParserResult result = asm_parser.parse(filename);

	if (!result.success)
	{
		printf("error: failed to parse the lsm\n");
		return 1;
	}

	SimpleVirtualMachine vm(128_KB);
	//vm.enableStackCheck(true); // uncomment to enable stack check, bad if using functions with unused return value

	try
	{
		vm.load(result.program);
	}
	catch (const SVMException &e)
	{
		error(e.what());
		return 1;
	}

	return vm.run() ? 0 : 1;
}