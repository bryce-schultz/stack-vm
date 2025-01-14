#pragma once
#include <cstdint>

#include "SimpleVirtualMachine.h"

typedef std::vector<uint64_t> Program;

#define ipush		(uint64_t)Instruction::PUSH
#define ipop		(uint64_t)Instruction::POP
#define iadd		(uint64_t)Instruction::ADD
#define isub		(uint64_t)Instruction::SUB
#define imul		(uint64_t)Instruction::MUL
#define idiv		(uint64_t)Instruction::DIV
#define imod		(uint64_t)Instruction::MOD
#define ipow		(uint64_t)Instruction::POW
#define ifact		(uint64_t)Instruction::FACT
#define inop		(uint64_t)Instruction::NOP
#define iprint		(uint64_t)Instruction::PRINT
#define ihalt		(uint64_t)Instruction::HALT