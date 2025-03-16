#pragma once
#include <exception>
#include <string>
#include <cstdint>

class SVMException : public std::exception
{
public:
	SVMException(const std::string& message) : message(message) {}
	const char* what() const noexcept override { return message.c_str(); }
private:
	std::string message;
};

class SVMStackOverflowException : public SVMException
{
public:
	SVMStackOverflowException() : SVMException("stack overflow") {}
};

class SVMStackUnderflowException : public SVMException
{
public:
	SVMStackUnderflowException() : SVMException("stack underflow") {}
};

class SVMInvalidInstructionException : public SVMException
{
public:
	SVMInvalidInstructionException(uint64_t instruction) : SVMException("invalid instruction: " + std::to_string(instruction)) {}
};

class SVMInvalidMemoryAccessException : public SVMException
{
public:
	SVMInvalidMemoryAccessException(uint64_t address) : SVMException("invalid memory access: " + std::to_string(address)) {}
};

class SVMProgramTooLargeException : public SVMException
{
public:
	SVMProgramTooLargeException() : SVMException("program too large") {}
};

class SVMDivideByZeroException : public SVMException
{
public:
	SVMDivideByZeroException() : SVMException("divide by zero") {}
};

class SVMModuloByZeroException : public SVMException
{
public:
	SVMModuloByZeroException() : SVMException("modulo by zero") {}
};

class SVMStackCorruptionException : public SVMException
{
public:
	SVMStackCorruptionException() : SVMException("stack corruption") {}
};

class SVMFactorialOfNegativeException : public SVMException
{
public:
	SVMFactorialOfNegativeException() : SVMException("factorial of negative number") {}
};