#include <fstream>
#include <string.h>
#include <string>
#include <vector>
#include <sstream>

#include "LithiumParser.h"
#include "Color.h"

#define error(message, token) _error(message, token, __FILE__, __LINE__)

LithiumParser::LithiumParser():
	tokenizer(),
	errors()
{
}

Node *LithiumParser::parse(const std::string &source)
{
	std::ifstream file(source);
	// if the file is not open, treat the source as a string
	if (!file.is_open())
	{
		return parseInternal(source);
	}

	// if the file is open, treat the source as a filename
	// and read the file into a string
	std::string line;
	std::string content;
	while (std::getline(file, line))
	{
		content += line + "\n";
	}

	file.close();
	return parseInternal(content, source);
}

std::vector<std::string> LithiumParser::getErrors() const
{
	return errors;
}

void LithiumParser::_error(const std::string &message, const Token &token, const char* file, size_t lineno)
{
	const std::string &filename = token.getLocation().getFilename();
	const std::string line = std::to_string(token.getLocation().getLine());
	const std::string column = std::to_string(token.getLocation().getColumn());

	bool showLocation = true;

	std::stringstream ss;

	if (file != nullptr && showLocation)
	{
		ss << red << "error " << reset << filename << ":" << line << ":" << column << ":\n|  " << message << "\n|  @ " << file << ":" << lineno;
	}
	else
	{
		ss << red << "error " << filename << ":" << line << ":" << column << ":\n|  " << message << reset;
	}

	errors.push_back(ss.str());
}

Node *LithiumParser::parseInternal(const std::string &source, const std::string &filename)
{
	tokenizer = LithiumTokenizer(source, filename);
	return parseProgram();
}

ProgramNode *LithiumParser::parseProgram()
{
	ProgramNode *program = new ProgramNode();

	while (!tokenizer.eof())
	{
		Token token = peekToken();
		if (token == END)
		{
			return program;
		}

		StatementNode *statement = parseStatement();

		if (statement == nullptr)
		{
			delete program;
			return nullptr;
		}

		program->addChild(statement);
	}

	return program;
}

StatementNode *LithiumParser::parseStatement()
{
	Token token = peekToken();

	if (token == PRINT)
	{
		PrintStatementNode *printStatement = parsePrintStatement();
		if (!printStatement)
		{
			return nullptr;
		}

		return printStatement;
	}

	if (token == ASM)
	{
		AsmStatementNode *asmStatement = parseAsmStatement();
		if (!asmStatement)
		{
			return nullptr;
		}

		return asmStatement;
	}

	if (token == NUMBER || token == STRING || token == '(' || token == '-')
	{
		ExpressionNode *expression = parseExpression();
		if (!expression)
		{
			return nullptr;
		}

		token = peekToken();

		if (token != ';')
		{
			nextToken();
			error("expected ';'", token);
			return nullptr;
		}

		nextToken();

		return new PrintStatementNode(expression);
	}

	error("unexpected token: " + token.getText() + " (" + std::to_string(token.getType()) + ")", token);
	return nullptr;
}

PrintStatementNode *LithiumParser::parsePrintStatement()
{
	Token token = peekToken();

	if (token != PRINT)
	{
		nextToken();
		error("expected 'print'", token);
		return nullptr;
	}

	nextToken();

	token = peekToken();

	if (token != '(')
	{
		nextToken();
		error("expected '('", token);
		return nullptr;
	}

	nextToken();

	ExpressionNode *expression = parseExpression();
	if (!expression)
	{
		error("print expects an expression", peekToken());
		return nullptr;
	}

	token = peekToken();

	if (token != ')')
	{
		nextToken();
		error("expected ')'", token);
		return nullptr;
	}

	token = nextToken();

	if (token != ';')
	{
		nextToken();
		error("expected ';'", token);
		return nullptr;
	}

	nextToken();

	return new PrintStatementNode(expression);
}

AsmStatementNode *LithiumParser::parseAsmStatement()
{
	Token token = peekToken();

	if (token != ASM)
	{
		return nullptr;
	}

	nextToken();

	token = peekToken();

	if (token != '(')
	{
		nextToken();
		error("expected '('", token);
		return nullptr;
	}

	nextToken();

	token = peekToken();

	if (token != STRING)
	{
		nextToken();
		error("expected string", token);
		return nullptr;
	}

	std::string value = token.getText();

	nextToken();

	token = peekToken();

	if (token != ')')
	{
		nextToken();
		error("expected ')'", token);
		return nullptr;
	}

	token = nextToken();

	if (token != ';')
	{
		nextToken();
		error("expected ';'", token);
		return nullptr;
	}

	nextToken();

	return new AsmStatementNode(new StringExpressionNode(value));
}

ExpressionNode *LithiumParser::parseExpression()
{
	Token token = peekToken();

	if (token == NUMBER || token == '(' || token == '-')
	{
		NumericExpressionNode *numericExpression = parseNumericExpression();
		if (!numericExpression)
		{
			return nullptr;
		}

		return numericExpression;
	}

	if (token == STRING)
	{
		StringExpressionNode *stringExpression = parseStringExpression();
		if (!stringExpression)
		{
			return nullptr;
		}

		return stringExpression;
	}

	error("expected a string or numeric expression", token);
	return nullptr;
}

NumericExpressionNode *LithiumParser::parseNumericExpression()
{
	NumericExpressionNode *addit = parseAddit();
	if (!addit)
	{
		return nullptr;
	}

	return addit;
}

StringExpressionNode *LithiumParser::parseStringExpression()
{
	Token token = peekToken();

	if (token != STRING)
	{
		return nullptr;
	}

	std::string value = token.getText();
	nextToken();

	StringExpressionNode *stringExpressionPP = parseStringExpressionPP(new StringExpressionNode(value));
	if (!stringExpressionPP)
	{
		return nullptr;
	}

	return stringExpressionPP;
}

StringExpressionNode *LithiumParser::parseStringExpressionP()
{
	Token token = peekToken();

	if (token == STRING)
	{
		nextToken();
		return new StringExpressionNode(token.getText());
	}

	if (token == NUMBER)
	{
		nextToken();
		return new StringExpressionNode(token.getText());
	}

	nextToken();
	error("expected string or number after concatenation operator", token);
	return nullptr;
}

StringExpressionNode *LithiumParser::parseStringExpressionPP(StringExpressionNode *lhs)
{
	Token token = peekToken();

	if (token == '+')
	{
		nextToken();
		StringExpressionNode *stringExpressionP = parseStringExpressionP();
		if (!stringExpressionP)
		{
			return nullptr;
		}

		auto concat = new ConcatNode(lhs, stringExpressionP);

		StringExpressionNode *stringExpressionPP = parseStringExpressionPP(concat);
		if (!stringExpressionPP)
		{
			return nullptr;
		}

		return stringExpressionPP;
	}

	return lhs;
}

NumericExpressionNode *LithiumParser::parseAddit()
{
	NumericExpressionNode *term = parseTerm();
	if (!term)
	{
		return nullptr;
	}

	NumericExpressionNode *additPP = parseAdditPP(term);
	if (!additPP)
	{
		return nullptr;
	}

	return additPP;
}

NumericExpressionNode *LithiumParser::parseAdditP(NumericExpressionNode *lhs)
{
	Token token = peekToken();

	if (token == '+' || token == '-')
	{
		char op = token;
		nextToken();

		NumericExpressionNode *term = parseTerm();

		if (!term)
		{
			return nullptr;
		}

		return new BinaryExpressionNode(lhs, op, term);
	}

	return nullptr;
}

NumericExpressionNode *LithiumParser::parseAdditPP(NumericExpressionNode *lhs)
{
	Token token = peekToken();

	if (token == '+' || token == '-')
	{
		NumericExpressionNode *additP = parseAdditP(lhs);
		if (!additP)
		{
			return nullptr;
		}

		NumericExpressionNode *additPP = parseAdditPP(additP);
		if (!additPP)
		{
			return nullptr;
		}

		return additPP;
	}

	return lhs;
}

NumericExpressionNode *LithiumParser::parseTerm()
{
	NumericExpressionNode *exponent = parseExponent();
	if (!exponent)
	{
		return nullptr;
	}

	NumericExpressionNode *termPP = parseTermPP(exponent);
	if (!termPP)
	{
		return nullptr;
	}

	return termPP;
}

NumericExpressionNode *LithiumParser::parseTermP(NumericExpressionNode *lhs)
{
	Token token = peekToken();

	if (token == '*' || token == '/' || token == '%')
	{
		char op = token;
		nextToken();

		NumericExpressionNode *exponent = parseExponent();
		if (!exponent)
		{
			return nullptr;
		}

		return new BinaryExpressionNode(lhs, op, exponent);
	}

	return nullptr;
}

NumericExpressionNode *LithiumParser::parseTermPP(NumericExpressionNode *lhs)
{
	Token token = peekToken();

	if (token == '*' || token == '/' || token == '%')
	{
		NumericExpressionNode *termP = parseTermP(lhs);
		if (!termP)
		{
			return nullptr;
		}

		NumericExpressionNode *termPP = parseTermPP(termP);
		if (!termPP)
		{
			return nullptr;
		}

		return termPP;
	}

	return lhs;
}

NumericExpressionNode *LithiumParser::parseExponent()
{
	NumericExpressionNode *fact = parseFact();
	if (!fact)
	{
		return nullptr;
	}

	NumericExpressionNode *exponentP = parseExponentP(fact);
	if (!exponentP)
	{
		return nullptr;
	}

	return exponentP;
}

NumericExpressionNode *LithiumParser::parseExponentP(NumericExpressionNode *lhs)
{
	Token token = peekToken();

	if (token == '^')
	{
		char op = token;
		nextToken();

		NumericExpressionNode *factorial = parseFactorial();
		if (!factorial)
		{
			return nullptr;
		}

		BinaryExpressionNode *exponent = new BinaryExpressionNode(lhs, op, factorial);

		NumericExpressionNode *exponentP = parseExponentP(exponent);
		if (!exponentP)
		{
			return nullptr;
		}

		return exponentP;
	}

	return lhs;
}

NumericExpressionNode *LithiumParser::parseFactorial()
{
	NumericExpressionNode *primary = parsePrimary();
	if (!primary)
	{
		return nullptr;
	}

	NumericExpressionNode *factorialP = parseFactorialP(primary);
	if (!factorialP)
	{
		return nullptr;
	}

	return factorialP;
}

NumericExpressionNode *LithiumParser::parseFactorialP(NumericExpressionNode *lhs)
{
	Token token = peekToken();

	if (token == '!')
	{
		char op = token;
		nextToken();

		UnaryExpressionNode *unary = new UnaryExpressionNode(lhs, op);

		NumericExpressionNode *factorialP = parseFactorialP(unary);
		if (!factorialP)
		{
			return nullptr;
		}

		return factorialP;
	}

	return lhs;
}

NumericExpressionNode *LithiumParser::parsePrimary()
{
	Token token = peekToken();

	if (token == '(')
	{
		nextToken();

		NumericExpressionNode *expression = parseNumericExpression();
		if (!expression)
		{
			return nullptr;
		}

		token = peekToken();
		if (token != ')')
		{
			return nullptr;
		}

		nextToken();
		return expression;
	}

	if (token == NUMBER)
	{
		nextToken();
		int value = std::stoi(token.getText());
		return new IntExpressionNode(value);
	}

	return nullptr;
}

NumericExpressionNode *LithiumParser::parseFact()
{
	Token token = peekToken();

	if (token == '-')
	{
		nextToken();

		NumericExpressionNode *factorial = parseFactorial();
		if (!factorial)
		{
			return nullptr;
		}

		return new UnaryExpressionNode(factorial, '-');
	}

	return parseFactorial();
}

Token LithiumParser::peekToken()
{
	return tokenizer.peekToken();
}

Token LithiumParser::nextToken()
{
	return tokenizer.nextToken();
}
