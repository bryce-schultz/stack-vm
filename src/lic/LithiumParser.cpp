#include <fstream>
#include <string.h>
#include <string>
#include <vector>
#include <sstream>

#include "LithiumParser.h"
#include "Error.h"
#include "Color.h"

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

Token LithiumParser::peekToken()
{
	return tokenizer.peekToken();
}

Token LithiumParser::nextToken()
{
	return tokenizer.nextToken();
}

Node *LithiumParser::parseInternal(const std::string &source, const std::string &filename)
{
	tokenizer = LithiumTokenizer(source, filename);
	return parseProgram();
}

ProgramNode *LithiumParser::parseProgram()
{
	ProgramNode *program = new ProgramNode();

	auto statements = parseStatements();
	if (!statements)
	{
		return nullptr;
	}

	program->addChild(statements);

	if (errors.size() > 0)
	{
		return nullptr;
	}

	return program;
}

StatementNode *LithiumParser::parseStatement()
{
	Token token = peekToken();

	if (token == END)
	{
		error("unexpected end of file", token);
		return nullptr;
	}

	if (token == '}' || token == ')') // end of block
	{
		return nullptr;
	}

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

	if (token == IDENTIFIER)
	{
		nextToken();
		return parseStatementP(token);
	}

	if (token == FOR)
	{
		return parseForStatement();
	}

	if (token == WHILE)
	{
		return parseWhileStatement();
	}

	if (token == IF)
	{
		return parseIfStatement();
	}

	if (token == '{')
	{
		BlockNode *block = parseBlock();
		if (!block)
		{
			return nullptr;
		}

		return block;
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

	if (token == NUMBER || token == '(' || token == '-' || token == IDENTIFIER)
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

	Token token = peekToken();

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

	if (token == '+' || token == '-' || token == '<' || token == '>' || token == EQUALS)
	{
		int op = token;
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

	if (token == '+' || token == '-' || token == '<' || token == '>' || token == EQUALS)
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

	if (token == IDENTIFIER)
	{
		nextToken();
		return new VariableExpressionNode(token);
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

DeclNode *LithiumParser::parseDeclaration()
{
	return parseVarDeclaration();
}

VarDeclNode *LithiumParser::parseVarDeclaration()
{
	Token token = peekToken();

	if (token != IDENTIFIER)
	{
		nextToken();
		error("expected identifier", token);
		return nullptr;
	}

	Token identifier = token;
	nextToken();

	token = peekToken();

	if (token == ';')
	{
		return new VarDeclNode(identifier, new IntExpressionNode(0));
	}

	if (token != '=')
	{
		nextToken();
		expected('=', token);
		return nullptr;
	}

	nextToken();

	ExpressionNode *expression = parseExpression();
	if (!expression)
	{
		return nullptr;
	}

	return new VarDeclNode(identifier, expression);
}

StatementNode *LithiumParser::parseStatementP(const Token &identifier)
{
	Token token = peekToken();

	if (token == ';')
	{
		nextToken();
		return new VarDeclNode(identifier, new IntExpressionNode(0));
	}

	if (token == '(')
	{
		nextToken();

		std::vector<ExpressionNode *> expressions;

		while (true)
		{
			ExpressionNode *expression = parseExpression();
			if (!expression)
			{
				return nullptr;
			}

			expressions.push_back(expression);

			token = peekToken();
			if (token == ')')
			{
				break;
			}

			if (token != ',')
			{
				nextToken();
				error("expected ','", token);
				return nullptr;
			}

			nextToken();
		}

		nextToken();

		token = peekToken();

		if (token != ';')
		{
			nextToken();
			error("expected ';'", token);
			return nullptr;
		}

		nextToken();

		//return new FuncCallNode(identifier, expressions);
		return nullptr;
	}

	if (token == '=')
	{
		nextToken();

		ExpressionNode *expression = parseExpression();
		if (!expression)
		{
			return nullptr;
		}

		token = peekToken();

		if (token == ';')
		{
			nextToken();
		}

		return new AssignNode(identifier, expression);
	}

	if (token == INCREMENT)
	{
		nextToken();

		token = peekToken();

		if (token == ';' || token == ')')
		{
			if (token == ';') nextToken();
			BinaryExpressionNode *add = new BinaryExpressionNode(new VariableExpressionNode(identifier), '+', new IntExpressionNode(1));
			AssignNode *assign = new AssignNode(identifier, add);

			return assign;
		}

		nextToken();

		error("expected ';'", token);
		return nullptr;
	}

	nextToken();
	expected("';', '=', or '('", token);
	return nullptr;
}

StatementsNode *LithiumParser::parseStatements()
{
	StatementsNode *statements = new StatementsNode();

    while (true)
	{
		Token token = peekToken();
		if (token == END)
		{
			break;
		}

		StatementNode *statement = parseStatement();
		if (statement == nullptr)
		{
			return statements;
		}

		statements->addChild(statement);
	}

	return statements;
}

ForStatementNode *LithiumParser::parseForStatement()
{
	Token token = peekToken();

	if (token != FOR)
	{
		nextToken();
		error("expected 'for'", token);
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

	global::symbolTable.increaseScope();

	StatementNode *init = parseAssignment();
	if (!init)
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

	ExpressionNode *condition = parseExpression();
	if (!condition)
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

	StatementNode *increment = parseStatement();
	if (!increment)
	{
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

	BlockNode *block = parseBlock();
	if (!block)
	{
		return nullptr;
	}

	global::symbolTable.decreaseScope();

	return new ForStatementNode(init, condition, increment, block);
}

BlockNode *LithiumParser::parseBlock()
{
	Token token = peekToken();

	if (token != '{')
	{
		nextToken();
		error("expected '{'", token);
		return nullptr;
	}

	global::symbolTable.increaseScope();

	nextToken();

	StatementsNode *statements = parseStatements();
	if (!statements)
	{
		return nullptr;
	}

	token = peekToken();

	if (token != '}')
	{
		nextToken();
		error("expected '}'", token);
		return nullptr;
	}

	nextToken();

	global::symbolTable.decreaseScope();

	return new BlockNode(statements);
}

AssignNode *LithiumParser::parseAssignment()
{
	Token token = peekToken();

	if (token != IDENTIFIER)
	{
		nextToken();
		error("expected identifier", token);
		return nullptr;
	}

	Token identifier = token;
	nextToken();

	token = peekToken();

	if (token == ';')
	{
		return new AssignNode(identifier, new IntExpressionNode(0));
	}

	if (token != '=')
	{
		nextToken();
		error("expected '='", token);
		return nullptr;
	}

	nextToken();

	ExpressionNode *expression = parseExpression();
	if (!expression)
	{
		return nullptr;
	}

	return new AssignNode(identifier, expression);
}

WhileStatementNode *LithiumParser::parseWhileStatement()
{
	Token token = peekToken();

	if (token != WHILE)
	{
		nextToken();
		error("expected 'while'", token);
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

	ExpressionNode *condition = parseExpression();
	if (!condition)
	{
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

	BlockNode *block = parseBlock();
	if (!block)
	{
		return nullptr;
	}

	return new WhileStatementNode(condition, block);
}

// IF ( numeric_expression ) block ifStatementP
IfStatementNode *LithiumParser::parseIfStatement()
{
	Token token = peekToken();

	if (token != IF)
	{
		nextToken();
		error("expected 'if'", token);
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

	NumericExpressionNode *condition = parseNumericExpression();
	if (!condition)
	{
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

	StatementNode *statement = parseStatement();
	if (!statement)
	{
		return nullptr;
	}

	StatementNode *ifStatementP = parseIfStatementP();

	return new IfStatementNode(condition, statement, ifStatementP);
}

StatementNode *LithiumParser::parseIfStatementP()
{
	Token token = peekToken();

	if (token != ELSE)
	{
		return nullptr;
	}

	nextToken();

	return parseStatement();
}
