#include <fstream>

#include "LithiumParser.h"

LithiumParser::LithiumParser():
	source(""),
	index(0),
	currentToken(NONE),
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

void LithiumParser::error(const std::string &message)
{
	errors.push_back("error " + filename + ":" + std::to_string(lineno) + ":" + std::to_string(colno) + ":\n" + message);
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

int LithiumParser::peekToken()
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

int LithiumParser::getToken()
{
	text.clear();

	if (index >= source.size())
	{
		return END;
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
			return END;
		}
		c = source[index];
	}

	// parse operators
	if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^' || c == '!')
	{
		index++;
		colno++;
		text = c;
		return c;
	}

	// parse parentheses
	if (c == '(' || c == ')')
	{
		index++;
		colno++;
		text = c;
		return c;
	}

	// parse numbers
	if (c >= '0' && c <= '9')
	{
		text.clear();
		while (c >= '0' && c <= '9')
		{
			text += c;
			index++;
			colno++;
			if (index >= source.size())
			{
				break;
			}
			c = source[index];
		}

		return NUMBER;
	}

	// parse identifiers
	if (c >= 'a' && c <= 'z')
	{
		text.clear();
		while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		{
			text += c;
			index++;
			colno++;
			if (index >= source.size())
			{
				break;
			}
			c = source[index];
		}

		if (text == "print")
		{
			return PRINT;
		}

		return IDENTIFIER;
	}

	return JUNK;
}

int LithiumParser::nextToken()
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
	int token = peekToken();

	if (token == PRINT)
	{
		PrintStatementNode *printStatement = parsePrintStatement();
		if (!printStatement)
		{
			return nullptr;
		}

		return printStatement;
	}

	return nullptr;
}

PrintStatementNode *LithiumParser::parsePrintStatement()
{
	int token = peekToken();

	if (token != PRINT)
	{
		return nullptr;
	}

	nextToken();

	token = peekToken();

	if (token != '(')
	{
		nextToken();
		error("expected '('");
		return nullptr;
	}

	nextToken();

	ExpressionNode *expression = parseExpression();

	token = peekToken();

	if (token != ')')
	{
		nextToken();
		error("expected ')'");
		return nullptr;
	}

	nextToken();

	return new PrintStatementNode(expression);
}

ExpressionNode *LithiumParser::parseExpression()
{
	return parseAddit();
}

ExpressionNode *LithiumParser::parseAddit()
{
	int token = peekToken();

	// TODO: add the - operator for unary minus here
	if (token == '-')
	{
		nextToken();
	}

	ExpressionNode *term = parseTerm();
	if (!term)
	{
		return nullptr;
	}

	ExpressionNode *additPP = parseAdditPP(term);
	if (!additPP)
	{
		return nullptr;
	}

	return additPP;
}

ExpressionNode *LithiumParser::parseAdditP(ExpressionNode *lhs)
{
	int token = peekToken();

	if (token == '+' || token == '-')
	{
		char op = token;
		nextToken();

		ExpressionNode *term = parseTerm();

		if (!term)
		{
			return nullptr;
		}

		return new BinaryExpressionNode(lhs, op, term);
	}

	return nullptr;
}

ExpressionNode *LithiumParser::parseAdditPP(ExpressionNode *lhs)
{
	int token = peekToken();

	if (token == '+' || token == '-')
	{
		ExpressionNode *additP = parseAdditP(lhs);
		if (!additP)
		{
			return nullptr;
		}

		ExpressionNode *additPP = parseAdditPP(additP);
		if (!additPP)
		{
			return nullptr;
		}

		return additPP;
	}

	return lhs;
}

ExpressionNode *LithiumParser::parseTerm()
{
	ExpressionNode *exponent = parseExponent();
	if (!exponent)
	{
		return nullptr;
	}

	ExpressionNode *termPP = parseTermPP(exponent);
	if (!termPP)
	{
		return nullptr;
	}

	return termPP;
}

ExpressionNode *LithiumParser::parseTermP(ExpressionNode *lhs)
{
	int token = peekToken();

	if (token == '*' || token == '/' || token == '%')
	{
		char op = token;
		nextToken();

		ExpressionNode *exponent = parseExponent();
		if (!exponent)
		{
			return nullptr;
		}

		return new BinaryExpressionNode(lhs, op, exponent);
	}

	return nullptr;
}

ExpressionNode *LithiumParser::parseTermPP(ExpressionNode *lhs)
{
	int token = peekToken();

	if (token == '*' || token == '/' || token == '%')
	{
		ExpressionNode *termP = parseTermP(lhs);
		if (!termP)
		{
			return nullptr;
		}

		ExpressionNode *termPP = parseTermPP(termP);
		if (!termPP)
		{
			return nullptr;
		}

		return termPP;
	}

	return lhs;
}

ExpressionNode *LithiumParser::parseExponent()
{
	ExpressionNode *factorial = parseFactorial();
	if (!factorial)
	{
		return nullptr;
	}

	ExpressionNode *exponentP = parseExponentP();
	if (!exponentP)
	{
		return nullptr;
	}

	return factorial;
}

ExpressionNode *LithiumParser::parseExponentP()
{
	int token = peekToken();

	if (token == '^')
	{
		nextToken();

		ExpressionNode *factorial = parseFactorial();
		if (!factorial)
		{
			return nullptr;
		}

		ExpressionNode *exponentP = parseExponentP();
		if (!exponentP)
		{
			return nullptr;
		}

		return factorial;
	}

	return new ExpressionNode();
}

ExpressionNode *LithiumParser::parseFactorial()
{
	ExpressionNode *primary = parsePrimary();
	if (!primary)
	{
		return nullptr;
	}

	ExpressionNode *factorialP = parseFactorialP(primary);
	if (!factorialP)
	{
		return nullptr;
	}

	return factorialP;
}

ExpressionNode *LithiumParser::parseFactorialP(ExpressionNode *lhs)
{
	int token = peekToken();

	if (token == '!')
	{
		char op = token;
		nextToken();

		UnaryExpressionNode *unary = new UnaryExpressionNode(lhs, op);

		ExpressionNode *factorialP = parseFactorialP(unary);
		if (!factorialP)
		{
			return nullptr;
		}

		return factorialP;
	}

	return lhs;
}

ExpressionNode *LithiumParser::parsePrimary()
{
	int token = peekToken();

	if (token == '(')
	{
		nextToken();

		ExpressionNode *expression = parseExpression();
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
