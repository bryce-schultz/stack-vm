#pragma once
#include <cstdint>

#include "SimpleVirtualMachine.h"

typedef std::vector<uint64_t> Program;

#define ipush       (uint64_t)Instruction::PUSH
#define ipop        (uint64_t)Instruction::POP
#define iadd        (uint64_t)Instruction::ADD
#define isub        (uint64_t)Instruction::SUB
#define imul        (uint64_t)Instruction::MUL
#define idiv        (uint64_t)Instruction::DIV
#define imod        (uint64_t)Instruction::MOD
#define ipow        (uint64_t)Instruction::POW
#define ineg        (uint64_t)Instruction::NEG
#define ifact       (uint64_t)Instruction::FACT
#define iinc        (uint64_t)Instruction::INC
#define idec        (uint64_t)Instruction::DEC
#define inop        (uint64_t)Instruction::NOP
#define iprint      (uint64_t)Instruction::PRINT
#define iprintstr   (uint64_t)Instruction::PRINTSTR
#define iconcat     (uint64_t)Instruction::CONCAT
#define ihalt       (uint64_t)Instruction::HALT
#define idup        (uint64_t)Instruction::DUP
#define ijmp        (uint64_t)Instruction::JMP
#define ijz         (uint64_t)Instruction::JZ
#define ijnz        (uint64_t)Instruction::JNZ
#define ije         (uint64_t)Instruction::JE
#define ijne        (uint64_t)Instruction::JNE
#define ijg         (uint64_t)Instruction::JG
#define ijge        (uint64_t)Instruction::JGE
#define ijl         (uint64_t)Instruction::JL
#define igt         (uint64_t)Instruction::GT
#define ige         (uint64_t)Instruction::GE
#define ilt         (uint64_t)Instruction::LT
#define ile         (uint64_t)Instruction::LE
#define ieq         (uint64_t)Instruction::EQ
#define ine         (uint64_t)Instruction::NE
#define ijle        (uint64_t)Instruction::JLE
#define iand        (uint64_t)Instruction::AND
#define ior         (uint64_t)Instruction::OR
#define istore      (uint64_t)Instruction::STORE
#define ipstore     (uint64_t)Instruction::PSTORE
#define iload       (uint64_t)Instruction::LOAD
#define istostr     (uint64_t)Instruction::STOSTR
#define iloadstr    (uint64_t)Instruction::LOADSTR
#define isyscall    (uint64_t)Instruction::SYSCALL
#define icall       (uint64_t)Instruction::CALL
#define iret        (uint64_t)Instruction::RET
#define iretval     (uint64_t)Instruction::RETVAL
