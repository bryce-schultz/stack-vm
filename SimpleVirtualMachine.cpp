#include <cstring>
#include <cstdio>

#include "SVMException.h"
#include "SimpleVirtualMachine.h"

constexpr uint64_t canary = 0xDEADBEEFDEADBEEF;

SimpleVirtualMachine::SimpleVirtualMachine(size_t memorySize):
	memory(new uint64_t[memorySize / sizeof(uint64_t)]),
	sp(memory),
	bp(memory),
	ip(memory),
	fp(memory),
	memorySize(memorySize / sizeof(uint64_t)),
	stackSize(0),
	isStackCheckEnabled(false)
{
}

SimpleVirtualMachine::~SimpleVirtualMachine()
{
	delete[] memory;

	memory = nullptr;
	sp = nullptr;
	ip = nullptr;
	fp = nullptr;
}

bool SimpleVirtualMachine::load(const uint64_t* program, size_t programSize)
{
	if (programSize > memorySize)
	{
		throw SVMProgramTooLargeException();
	}

	memcpy(memory, program, programSize * sizeof(uint64_t));

	// set the canary value
	memory[programSize] = canary;

	ip = memory;
	sp = computeStackStart(programSize + 1);
	bp = sp;
	fp = sp;

	return true;
}

bool SimpleVirtualMachine::load(const std::vector<uint64_t> &program)
{
	return load(program.data(), program.size());
}

bool SimpleVirtualMachine::run()
{
	bool result = runInternal();

	if (!result) isStackCheckEnabled = true;

	checkStack();

	return result;
}

bool SimpleVirtualMachine::runInternal()
{
	bool result = true;
	while (result)
	{
		uint64_t instruction = fetch();
		try
		{
			result = execute(instruction);
		}
		catch (const SVMException& e)
		{
			printf("error: %s\n", e.what());
			return false;
		}
	}

	return true;
}

void SimpleVirtualMachine::push(uint64_t value)
{
	if (sp >= memory + memorySize)
	{
		throw SVMStackOverflowException();
	}

	*sp = value;
	sp++;
}

uint64_t SimpleVirtualMachine::pop()
{
	if (sp <= bp)
	{
		throw SVMStackUnderflowException();
	}

	sp--;
	return *sp;
}

uint64_t SimpleVirtualMachine::fetch()
{
	if (ip >= memory + memorySize)
	{
		throw SVMInvalidMemoryAccessException(reinterpret_cast<uint64_t>(ip));
	}

	return *ip++;
}

size_t SimpleVirtualMachine::computeStackSize(size_t programSize, size_t heapSize) const
{
	return memorySize - programSize - heapSize;
}

uint64_t *SimpleVirtualMachine::computeStackStart(size_t programSize, size_t heapSize) const
{
	return memory + programSize + heapSize;
}

bool SimpleVirtualMachine::execute(uint64_t instruction)
{
	switch (static_cast<Instruction>(instruction))
	{
		case Instruction::PUSH:
		{
			uint64_t value = fetch();
			push(value);
			break;
		}
		case Instruction::POP:
		{
			pop();
			break;
		}
		case Instruction::ADD:
		{
			uint64_t right = pop();
			uint64_t left = pop();
			push(left + right);
			break;
		}
		case Instruction::SUB:
		{
			uint64_t right = pop();
			uint64_t left = pop();
			push(left - right);
			break;
		}
		case Instruction::MUL:
		{
			uint64_t right = pop();
			uint64_t left = pop();
			push(left * right);
			break;
		}
		case Instruction::DIV:
		{
			uint64_t right = pop();
			uint64_t left = pop();

			if (right == 0)
			{
				throw SVMDivideByZeroException();
			}

			push(left / right);
			break;
		}
		case Instruction::MOD:
		{
			uint64_t right = pop();
			uint64_t left = pop();

			if (right == 0)
			{
				throw SVMModuloByZeroException();
			}

			push(left % right);
			break;
		}
		case Instruction::POW:
		{
			uint64_t right = pop();
			uint64_t left = pop();
			uint64_t result = 1;

			for (uint64_t i = 0; i < right; i++)
			{
				result *= left;
			}

			push(result);
			break;
		}
		case Instruction::FACT:
		{
			uint64_t value = pop();
			uint64_t result = 1;

			for (uint64_t i = 1; i <= value; i++)
			{
				result *= i;
			}

			push(result);
			break;
		}
		case Instruction::NOP:
		{
			break;
		}
		case Instruction::PRINT:
		{
			uint64_t value = pop();
			printf("%llu\n", value);
			break;
		}
		case Instruction::HALT:
		{
			return false;
		}
		default:
		{
			if (instruction == canary)
			{
				throw SVMInvalidMemoryAccessException(reinterpret_cast<uint64_t>(ip));
			}
			throw SVMInvalidInstructionException(instruction);
		}
	}

	return true;
}

bool SimpleVirtualMachine::checkStack()
{
	if (isStackCheckEnabled)
	{
		// check if the stack pointer is reset to the base pointer
		if (sp != bp)
		{
			printf("[STACK CHECK] stack corruption detected\n");
			return false;
		}
	}

	return true;
}

void SimpleVirtualMachine::enableStackCheck(bool enable)
{
	isStackCheckEnabled = enable;
}
