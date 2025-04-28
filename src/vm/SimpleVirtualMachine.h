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
	NEG,
	POW,
	FACT,
	INC,
	DEC,
	PRINT,
	PRINTSTR,
	CONCAT,
	JMP,
	JZ,
	JNZ,
	JE,
	JNE,
	JGT,
	JGE,
	JLT,
	JLE,
	GT,
	GE,
	LT,
	LE,
	EQ,
	NE,
	AND,
	OR,
	DUP,
	STORE,
	PSTORE,
	LOAD,
	STOSTR,
	LOADSTR,
	SYSCALL,
	CALL,
	RET,
	RETVAL
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
	uint64_t *computeHeapStart(size_t programSize) const;

	bool execute(uint64_t instruction);

	bool checkStack();
	bool memorySizeCheck(const uint64_t *program, size_t programSize);

	// syscall handler
	// returns true if the syscall was handled, false otherwise
	// the syscall id and arguments are passed on the stack
	//
	// the syscall id is popped off the stack and then the
	// appropriate syscall is called which will handle popping
	// its own arguments off the stack
	//
	// the return value of the syscall is pushed back onto the stack
	// if that syscall has a return value
	//
	// SYSCALL 0 - exit
	// SYSCALL 1 - write
	// SYSCALL 2 - read
	// SYSCALL 3 - open
	// SYSCALL 4 - close
	bool syscall();

	// syscall implementations
	// syscalls return true if the syscall was called
	// they return false if a fatal error occurred, but not necessarily
	// if the syscall itself failed

	// write (0)
	// arguments:
	// fd - file descriptor
	// buffer - buffer to write
	// size - number of bytes to write
	// returns:
	// the number of bytes written
	bool write();

	// read (1)
	// arguments:
	// fd - file descriptor
	// buffer - buffer to read into
	// size - number of bytes to read
	// returns:
	// the number of bytes read
	bool read();

	// open (2)
	// arguments:
	// path_length - length of the path string
	// path - path to the file
	// mode - file mode
	// returns:
	// the file descriptor
	bool open();

	// close (3)
	// arguments:
	// fd - file descriptor
	// returns:
	// 0 on success, -1 on failure
	bool close();

	// read a string off the stack
	// strings are defined as a length followed by
	// the characters and are not null terminated
	std::string readString();
private:
	uint64_t *memory; // the entire memory space
	uint64_t *sp; // stack pointer
	uint64_t *bp; // base pointer
	uint64_t *fp; // frame pointer
	uint64_t *ip; // instruction pointer
	uint64_t *heap; // heap base pointer

	size_t memorySize;
	size_t heapSize;
	size_t stackSize;
private:
	// track if the stack check is enabled
	// checks:
	// 1. stack pointer is equal to the base pointer (ensure stack is empty)
	bool isStackCheckEnabled;
};