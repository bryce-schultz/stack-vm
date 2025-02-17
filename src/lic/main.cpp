#include <cstdio>
#include <cstdint>

#include "LithiumCompiler.h"

constexpr uint8_t requiredArgCount = 2;

int main(int argc, char **argv)
{
	if (argc != requiredArgCount)
	{
		printf("usage: %s <source>\n", argv[0]);
		return EXIT_FAILURE;
	}

	LithiumCompiler compiler;

	if (!compiler.compile(argv[1]))
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
