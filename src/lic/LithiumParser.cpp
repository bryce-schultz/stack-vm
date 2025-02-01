#include <fstream>
#include <string.h>

#include "LithiumParser.h"

LithiumParser::LithiumParser():
	source(""),
	index(0),
	currentToken(NONE, "", {0, 0, ""}),
	text(""),
	colno(0),
	lineno(0),
	errors()
{
}

LithiumParser::~LithiumParser()
{
}

Node *LithiumParser::parse(const std::string &source)
{
	std::ifstream file(source);
	if (!file.is_open())
	{
		filename = "";
		return parseInternal(source);
	}

	filename = source;

	std::string result;
	std::string line;
	while (std::getline(file, line))
	{
		result += line + '\n';
	}

	file.close();
	return parseInternal(result);
}

std::vector<std::string> LithiumParser::getErrors() const
{
	return errors;
}

void LithiumParser::error(const std::string &message, const Token &token)
{
	//errors.push_back("error: " + message);
	errors.push_back("error " + filename + ":" + std::to_string(token.getLocation().getLine()) + ":" + std::to_string(token.getLocation().getColumn()) + ":\n" + message);
}

Node *LithiumParser::parseInternal(const std::string &source)
{
	this->source = source;
	index = 0;
	lineno = 1;
	colno = 1;

	ProgramNode *program = parseProgram();
	if (program == nullptr)
	{
		return nullptr;
	}

	return program;
}

Token LithiumParser::peekToken()
{
	if (currentToken == NONE)
	{
		int save_lineno = lineno;
		int save_colno = colno;
		currentToken = nextToken();
		lineno = save_lineno;
		colno = save_colno;
	}

	return currentToken;
}

Token LithiumParser::getToken()
{
	oldText = text;
	text.clear();

	if (index >= source.size())
	{
		return { END, "", {lineno, colno, filename} };
	}

	char c = source[index];

	// skip whitespace
	while (isWhitespace(c))
	{
		if (c == '\n')
		{
			lineno++;
			colno = 0;
		}
		index++;
		colno++;
		if (index >= source.size())
		{
			return { END, "", {lineno, colno, filename} };
		}
		c = source[index];
	}

	// skip comments
	if (c == '#')
	{
		while (c != '\n' && index < source.size())
		{
			index++;
			colno++;
			c = source[index];
		}
		lineno++;
		return getToken();
	}

	// parse operators and parentheses
	if (strchr("+-*/%^!()", c))
	{
		index++;
		colno++;
		text = c;
		return { c, text, {lineno, colno, filename} };
	}

	// parse numbers
	if (isdigit(c))
	{
		while (isdigit(c) && index < source.size())
		{
			text += c;
			index++;
			colno++;
			c = source[index];
		}
		return { NUMBER, text, {lineno, colno, filename} };
	}

	// parse identifiers and keywords
	if (isalpha(c))
	{
		while ((isalpha(c) || isdigit(c)) && index < source.size())
		{
			text += c;
			index++;
			colno++;
			c = source[index];
		}

		if (text == "print")
		{
			return { PRINT, text, {lineno, colno, filename} };
		}

		if (text == "asm")
		{
			return { ASM, text, {lineno, colno, filename} };
		}

		return { IDENTIFIER, text, {lineno, colno, filename} };
	}

	// parse strings
	if (c == '"')
	{
		index++;
		colno++;
		while (index < source.size() && (c = source[index]) != '"')
		{
			text += c;
			index++;
			colno++;
		}
		index++;
		colno++;

		return { STRING, text, {lineno, colno, filename} };
	}

	text = c;
	return { JUNK, text, {lineno, colno, filename} };
}

Token LithiumParser::nextToken()
{
	currentToken = getToken();
	return currentToken;
}

bool LithiumParser::isWhitespace(char c) const
{
	return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

ProgramNode *LithiumParser::parseProgram()
{
	ProgramNode *program = new ProgramNode();

	while (index < source.size())
	{
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

	if (token == NUMBER || token == STRING || token == '(')
	{
		ExpressionNode *expression = parseExpression();
		if (!expression)
		{
			return nullptr;
		}

		return new PrintStatementNode(expression);
	}

	error("unexpected token: " + text, token);
	return nullptr;
}

PrintStatementNode *LithiumParser::parsePrintStatement()
{
	Token token = peekToken();

	if (token != PRINT)
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

	ExpressionNode *expression = parseExpression();
	if (!expression)
	{
		error("print expects an expression", currentToken);
		return nullptr;
	}

	token = peekToken();

	if (token != ')')
	{
		nextToken();
		error("expected ')'", token);
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

	std::string value = text;

	nextToken();

	token = peekToken();

	if (token != ')')
	{
		nextToken();
		error("expected ')'", token);
		return nullptr;
	}

	nextToken();

	return new AsmStatementNode(new StringExpressionNode(value));
}

ExpressionNode *LithiumParser::parseExpression()
{
	Token token = peekToken();

	if (token == NUMBER || token == '(')
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

	std::string value = text;
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
		std::string value = text;
		nextToken();

		return new StringExpressionNode(value);
	}

	if (token == NUMBER)
	{
		std::string number = text;
		nextToken();

		return new StringExpressionNode(number);
	}

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
	Token token = peekToken();

	// TODO: add the - operator for unary minus here
	if (token == '-')
	{
		nextToken();
	}

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
	NumericExpressionNode *factorial = parseFactorial();
	if (!factorial)
	{
		return nullptr;
	}

	NumericExpressionNode *exponentP = parseExponentP(factorial);
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

		int value = std::stoi(text);
		nextToken();
		return new IntExpressionNode(value);
	}

	return nullptr;
}
