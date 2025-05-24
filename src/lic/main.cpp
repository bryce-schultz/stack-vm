//***********************************************
// main.cpp
//***********************************************

#include <cstdio>
#include <cstdint>

#include "LithiumCompiler.h"
#include "Error.h"
#include "Options.h"

constexpr uint8_t requiredArgCount = 2;

int main(int argc, char **argv)
{
	parseOptions(argc, argv);
	#ifdef DEBUG
		const char *filename = "test.li";
	#else
		if (global::options.inputFile.empty())
		{
			general_error("no input file specified");
			return EXIT_FAILURE;
		}
		const char *filename = global::options.inputFile.c_str();
	#endif

	LithiumCompiler compiler;

	if (!compiler.compile(filename))
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
