#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>

#include "SVMException.h"
#include "SimpleVirtualMachine.h"

constexpr uint64_t canary = 0xDEADBEEFDEADBEEF;

SimpleVirtualMachine::SimpleVirtualMachine(size_t memorySize):
	memory(new uint64_t[memorySize / sizeof(uint64_t)]),
	sp(memory),
	bp(memory),
	fp(memory),
	ip(memory),
	heap(memory),
	memorySize(memorySize / sizeof(uint64_t)),
	heapSize(0),
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

bool SimpleVirtualMachine::load(const uint64_t *program, size_t programSize)
{
	if (programSize > memorySize || !memorySizeCheck(program, programSize))
	{
		throw SVMProgramTooLargeException();
	}

	memcpy(memory, program, programSize * sizeof(uint64_t));

	// set the canary value
	memory[programSize++] = canary;
	float remainingMemory = memorySize - programSize;

	float heapPercent = 0.80;
	heapSize = remainingMemory * heapPercent;
	stackSize = computeStackSize(programSize, heapSize);

	ip = memory;
	heap = computeHeapStart(programSize);
	sp = computeStackStart(programSize, heapSize);
	bp = sp;
	fp = sp;

	/* good for debugging memory layout issues
	printf("bp: %ld\n", bp - memory);
	printf("sp: %ld\n", sp - memory);
	printf("fp: %ld\n", fp - memory);
	printf("ip: %ld\n", ip - memory);
	printf("heap: %ld\n", heap - memory);
	printf("stack size: %ld\n", stackSize);
	printf("heap size: %ld\n", heapSize);
	printf("program size: %ld\n", programSize);
	printf("memory size: %ld\n", memorySize);
	*/

	return true;
}

bool SimpleVirtualMachine::load(const std::vector<uint64_t> &program)
{
	return load(program.data(), program.size());
}

bool SimpleVirtualMachine::run()
{
	bool result = runInternal();

	if (!result)
		isStackCheckEnabled = true;

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
		catch (const SVMException &e)
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
		case Instruction::NEG:
		{
			uint64_t value = pop();
			push(-value);
			break;
		}
		case Instruction::FACT:
		{
			int64_t value = pop();

			if (value < 0)
			{
				throw SVMFactorialOfNegativeException();
			}

			uint64_t result = 1;

			for (int64_t i = 1; i <= value; i++)
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
			printf("%ld", value);
			fflush(stdout);
			break;
		}
		case Instruction::PRINTSTR:
		{
			uint64_t length = pop();
			for (uint64_t i = 0; i < length; i++)
			{
				uint64_t value = pop();

				if (value == '\\')
				{
					length--;
					value = pop();
					switch (value)
					{
						case 'n':
						{
							printf("\n");
							break;
						}
						case 't':
						{
							printf("\t");
							break;
						}
						default:
						{
							printf("%c", static_cast<char>(value));
							break;
						}
					}
					continue;
				}

				printf("%c", static_cast<char>(value));
			}
			fflush(stdout);
			break;
		}
		case Instruction::CONCAT:
		{
			std::string right;
			std::string left;

			uint64_t length = pop();

			for (uint64_t i = 0; i < length; i++)
			{
				uint64_t value = pop();
				right += static_cast<char>(value);
			}

			length = pop();

			for (uint64_t i = 0; i < length; i++)
			{
				uint64_t value = pop();
				left += static_cast<char>(value);
			}

			std::string result = left + right;

			for (ssize_t i = result.size() - 1; i >= 0; i--)
			{
				push(result[i]);
			}

			push(result.size());
			break;
		}
		case Instruction::DUP:
		{
			uint64_t value = pop();
			push(value);
			push(value);
			break;
		}
		case Instruction::JMP:
		{
			int64_t offset = fetch();
			ip += offset;
			break;
		}
		case Instruction::JZ:
		{
			int64_t offset = fetch();
			uint64_t value = pop();
			if (value == 0)
			{
				ip += offset;
			}
			break;
		}
		case Instruction::JNZ:
		{
			int64_t offset = fetch();
			uint64_t value = pop();
			if (value != 0)
			{
				ip += offset;
			}
			break;
		}
		case Instruction::JE:
		{
			int64_t offset = fetch();
			uint64_t right = pop();
			uint64_t left = pop();
			if (left == right)
			{
				ip += offset;
			}
			break;
		}
		case Instruction::JNE:
		{
			int64_t offset = fetch();
			uint64_t right = pop();
			uint64_t left = pop();
			if (left != right)
			{
				ip += offset;
			}
			break;
		}
		case Instruction::JG:
		{
			int64_t offset = fetch();
			uint64_t right = pop();
			uint64_t left = pop();
			if (left > right)
			{
				ip += offset;
			}
			break;
		}
		case Instruction::JGE:
		{
			int64_t offset = fetch();
			uint64_t right = pop();
			uint64_t left = pop();
			if (left >= right)
			{
				ip += offset;
			}
			break;
		}
		case Instruction::JL:
		{
			int64_t offset = fetch();
			uint64_t right = pop();
			uint64_t left = pop();
			if (left < right)
			{
				ip += offset;
			}
			break;
		}
		case Instruction::JLE:
		{
			int64_t offset = fetch();
			uint64_t right = pop();
			uint64_t left = pop();
			if (left <= right)
			{
				ip += offset;
			}
			break;
		}
		case Instruction::GT:
		{
			uint64_t right = pop();
			uint64_t left = pop();
			push(left > right);
			break;
		}
		case Instruction::LT:
		{
			uint64_t right = pop();
			uint64_t left = pop();
			push(left < right);
			break;
		}
		case Instruction::STORE:
		{
			uint64_t address = fetch();
			uint64_t value = pop();
			*(heap + address) = value;
			break;
		}
		case Instruction::LOAD:
		{
			uint64_t address = fetch();

			// check if the address is within the heap
			if (address >= heapSize)
			{
				throw SVMInvalidMemoryAccessException(address);
			}

			// memory access
			uint64_t value = *(heap + address);
			push(value);
			break;
		}
		case Instruction::SYSCALL:
		{
			// peek the syscall id and if its EXIT then return false
			int id = pop();
			if (static_cast<Syscall>(id) == Syscall::EXIT)
			{
				// pop the status code
				pop();
				return false;
			}

			push(id);
			return syscall();
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

bool SimpleVirtualMachine::memorySizeCheck(const uint64_t *program, size_t programSize)
{
	int64_t stack_size_needed = 0;

	for (size_t i = 0; i < programSize; i++)
	{
		switch (static_cast<Instruction>(program[i]))
		{
			case Instruction::PUSH:
			{
				stack_size_needed++;
				break;
			}
			case Instruction::POP:
			{
				stack_size_needed--;
				break;
			}
			default:
			{
				break;
			}
		}

		if (stack_size_needed < 0)
		{
			return false;
		}
	}

	if (stack_size_needed * sizeof(uint64_t) > memorySize)
	{
		return false;
	}

	return true;
}

bool SimpleVirtualMachine::syscall()
{
	uint64_t id = pop();

	switch (static_cast<Syscall>(id))
	{
		case Syscall::WRITE:
		{
			return write();
		}
		case Syscall::READ:
		{
			return read();
		}
		case Syscall::OPEN:
		{
			return open();
		}
		case Syscall::CLOSE:
		{
			return close();
		}
		default:
		{
			return false;
		}
	}

	return false;
}

bool SimpleVirtualMachine::write()
{
	std::string buffer;
	uint64_t size = pop();
	// avoid reallocations as the string is built off the stack
	buffer.reserve(size);
	for (uint64_t i = 0; i < size; i++)
	{
		uint64_t value = pop();
		buffer += static_cast<char>(value);
	}

	uint64_t fd = pop();
	ssize_t bytes_written = ::write(fd, buffer.c_str(), size);
	push(bytes_written);

	return true;
}

bool SimpleVirtualMachine::read()
{
	uint64_t size = pop();
	uint64_t fd = pop();

	std::string buffer(size, 0);
	ssize_t bytes_read = ::read(fd, &buffer[0], size);

	for (ssize_t i = bytes_read - 1; i >= 0; i--)
	{
		push(buffer[i]);
	}

	push(bytes_read);
	return true;
}

bool SimpleVirtualMachine::open()
{
	uint64_t flags = pop();
	uint64_t mode = pop();
	uint64_t path_length = pop();
	std::string path;

	for (uint64_t i = 0; i < path_length; i++)
	{
		uint64_t value = pop();
		path += static_cast<char>(value);
	}

	int fd = ::open(path.c_str(), flags, mode);
	if (fd == -1)
	{
		return false;
	}

	push(fd);
	return true;
}

bool SimpleVirtualMachine::close()
{
	uint64_t fd = pop();
	int result = ::close(fd);
	if (result == -1)
	{
		return false;
	}

	return true;
}

std::string SimpleVirtualMachine::readString()
{
    std::string buffer;
	uint64_t size = pop();
	// avoid reallocations as the string is built off the stack
	buffer.reserve(size);
	for (uint64_t i = 0; i < size; i++)
	{
		uint64_t value = pop();
		buffer += static_cast<char>(value);
	}

	return buffer;
}

void SimpleVirtualMachine::enableStackCheck(bool enable)
{
	isStackCheckEnabled = enable;
}

uint64_t *SimpleVirtualMachine::computeHeapStart(size_t programSize) const
{
	return memory + programSize;
}
