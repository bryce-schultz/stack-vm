//***********************************************
// main.cpp
//***********************************************

#include <cstdio>
#include <cstdint>

#include "LithiumCompiler.h"
#include "Error.h"

constexpr uint8_t requiredArgCount = 2;

int main(int argc, char **argv)
{
	#ifdef DEBUG
		const char *filename = "test.li";
	#else
		if (argc != requiredArgCount)
		{
			general_error("usage: lic <filename>");
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
