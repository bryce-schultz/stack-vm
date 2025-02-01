#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <string>

#include "LithiumParser.h"
#include "GeneratorVisitor.h"
#include "SimpleVirtualMachine.h"
#include "Util.h"

constexpr const char* const outputExtension = ".lsm";

bool runInterpreter(const std::string &filename);
bool runInterpreter();

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        return runInterpreter() ? EXIT_SUCCESS : EXIT_FAILURE;
    }

    if (argc != 2)
    {
        printf("usage: %s <source>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (!runInterpreter(argv[1]))
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

bool runInterpreter(const std::string &filename)
{
	LithiumParser liParser;

	Node *root = liParser.parse(filename);

	if (root == nullptr)
	{
		const auto &errors = liParser.getErrors();
		for (const auto &error : errors)
		{
			printf("%s\n", error.c_str());
		}

		if (errors.empty())
		{
			printf("error: failed to parse the source\n");
		}

		return false;
	}

	GeneratorVisitor generator;
	generator.visitAllChildren(root);

    SVASMParser asmParser;
	SVASMParserResult result = asmParser.parse(generator.getOutput());

	if (!result.success)
	{
		printf("error: failed to parse the lsm\n");
		return 1;
	}

    SimpleVirtualMachine vm(128_MB);

    try
    {
        vm.load(result.program);
    }
    catch (const SVMException &e)
    {
        printf("error: %s\n", e.what());
        return false;
    }

    return vm.run();
}

bool runInterpreter()
{
    const int bufferSize = 1024;
    char buffer[bufferSize];
    while (fgets(buffer, bufferSize, stdin) != nullptr)
    {

        if (strncmp(buffer, "exit\n", bufferSize) == 0)
        {
            return true;
        }

        LithiumParser liParser;
        Node *root = liParser.parse(buffer);

        if (root == nullptr)
        {
            const auto &errors = liParser.getErrors();
            for (const auto &error : errors)
            {
                printf("%s\n", error.c_str());
            }

            if (errors.empty())
            {
                printf("error: failed to parse the source\n");
            }

            continue;
        }

        GeneratorVisitor generator;
        generator.visitAllChildren(root);

        SVASMParser asmParser;
        SVASMParserResult result = asmParser.parse(generator.getOutput());

        if (!result.success)
        {
            printf("error: failed to parse the lsm\n");
            return 1;
        }

        SimpleVirtualMachine vm(128_MB);

        try
        {
            vm.load(result.program);
        }
        catch (const SVMException &e)
        {
            printf("error: %s\n", e.what());
            return false;
        }

        if (!vm.run())
        {
            return false;
        }
    }

    return true;
}
