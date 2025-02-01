#pragma once
#include <cstdint>
#include <vector>
#include <cstring>
#include <string>

#include "SVMException.h"
#include "SVASMParser.h"

// define a literal operator for memory sizes
constexpr size_t operator"" _KB(unsigned long long size)
{
	return size * 1024;
}

constexpr size_t operator"" _MB(unsigned long long size)
{
	return size * 1024 * 1024;
}

constexpr size_t operator"" _GB(unsigned long long size)
{
	return size * 1024 * 1024 * 1024;
}

enum class Instruction : uint64_t
{
	NOP = 0,
	HALT,
	PUSH,
	POP,
	ADD,
	SUB,
	MUL,
	DIV,
	MOD,
	POW,
	FACT,
	PRINT,
	PRINTSTR,
	CONCAT,
	SYSCALL
};

enum class Syscall : uint64_t
{
	EXIT = 0,
	WRITE,
	READ,
	OPEN,
	CLOSE
};

class SimpleVirtualMachine
{
public:
	SimpleVirtualMachine(size_t memorySize);
	~SimpleVirtualMachine();

	bool load(const uint64_t *program, size_t programSize);
	bool load(const std::vector<uint64_t> &program);

	bool run();

	void enableStackCheck(bool enable);

private:
	bool runInternal();
	void push(uint64_t value);
	uint64_t pop();
	uint64_t fetch();
	size_t computeStackSize(size_t programSize, size_t heapSize = 0) const;
	uint64_t *computeStackStart(size_t programSize, size_t heapSize = 0) const;

	bool execute(uint64_t instruction);

	bool checkStack();
	bool memorySizeCheck(const uint64_t *program, size_t programSize);

	// syscall handler
	// returns true if the syscall was handled, false otherwise
	// the syscall id is passed in the instruction stream
	// the syscall arguments are passed on the stack
	//
	// SYSCALL 0 - exit
	// SYSCALL 1 - write
	// SYSCALL 2 - read
	// SYSCALL 3 - open
	// SYSCALL 4 - close
	bool syscall();

	// syscall implementations
	// fd, buffer, size
	// returns the number of bytes written
	bool write();
	bool read();
	bool open();
	bool close();

private:
	uint64_t *memory; // the entire memory space
	uint64_t *sp; // stack pointer
	uint64_t *bp; // base pointer
	uint64_t *fp; // frame pointer
	uint64_t *ip; // instruction pointer

	size_t memorySize;
	size_t stackSize;

private:
	bool isStackCheckEnabled;
};