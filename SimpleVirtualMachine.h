#pragma once
#include <cstdint>
#include <vector>
#include <cstring>
#include <string>

// define a literal operator for memory sizes
constexpr size_t operator"" KB(unsigned long long size)
{
	return size * 1024;
}

constexpr size_t operator"" MB(unsigned long long size)
{
	return size * 1024 * 1024;
}

constexpr size_t operator"" GB(unsigned long long size)
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
	PRINT
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
private:
	uint64_t *memory;
	uint64_t *sp;
	uint64_t *bp;
	uint64_t *ip;
	uint64_t *fp;

	size_t memorySize;
	size_t stackSize;
private:
	bool isStackCheckEnabled;
};