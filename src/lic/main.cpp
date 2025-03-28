//***********************************************
// main.cpp
//***********************************************

#include <cstdio>
#include <cstdint>

#include "LithiumCompiler.h"
#include "Error.h"

constexpr uint8_t requiredArgCount = 2;

//#define DEBUG

int main(int argc, char **argv)
{
	#ifdef DEBUG
		const char *filename = "test.li";
	#else
		if (argc != requiredArgCount)
		{
			printf("usage: %s <source>\n", argv[0]);
			return EXIT_FAILURE;
		}
		const char *filename = argv[1];
	#endif

	LithiumCompiler compiler;

	if (!compiler.compile(filename))
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
