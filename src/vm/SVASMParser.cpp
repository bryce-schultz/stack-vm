#include <fstream>

#include "SVASMParser.h"
#include "Error.h"

SVASMParserResult SVASMParser::parseInternal(const std::string& source)
{
	SVASMParserResult result;
	result.success = true;

	std::vector<std::string> tokens;
	std::string token;

    for (size_t i = 0; i < source.size(); i++)
    {
        if (source[i] == '#' || source[i] == '.')
        {
            while (i < source.size() && source[i] != '\n')
            {
                i++;
            }
            continue;
        }

        if (isWhitespace(source[i]))
        {
            if (token.empty())
            {
				continue;
            }

			// check if the token is a label
			if (token.back() == ':')
			{
				token.pop_back();
				symbolTable.add(token, tokens.size());
				token.clear();
			}
			else
			{
				tokens.push_back(token);
				token.clear();
			}
        }
        else
        {
            token += tolower(source[i]);
        }
    }

	if (!token.empty())
	{
		tokens.push_back(token);
	}

	for (size_t i = 0; i < tokens.size(); i++)
	{
		if (tokens[i] == "push")
		{
			if (i + 1 >= tokens.size())
			{
				result.success = false;
				return result;
			}
			result.program.push_back(ipush);
			try
			{
				uint64_t value = std::stoull(tokens[++i]);
				result.program.push_back(value);
			}
			catch (const std::exception&)
			{
				result.success = false;
				printf("error: invalid number '%s'\n", tokens[i].c_str());
				return result;
			}
		}
		else if (tokens[i] == "pop")
		{
			result.program.push_back(ipop);
		}
		else if (tokens[i] == "add")
		{
			result.program.push_back(iadd);
		}
		else if (tokens[i] == "sub")
		{
			result.program.push_back(isub);
		}
		else if (tokens[i] == "mul")
		{
			result.program.push_back(imul);
		}
		else if (tokens[i] == "div")
		{
			result.program.push_back(idiv);
		}
		else if (tokens[i] == "mod")
		{
			result.program.push_back(imod);
		}
		else if (tokens[i] == "pow")
		{
			result.program.push_back(ipow);
		}
		else if (tokens[i] == "neg")
		{
			result.program.push_back(ineg);
		}
		else if (tokens[i] == "fact")
		{
			result.program.push_back(ifact);
		}
		else if (tokens[i] == "inc")
		{
			result.program.push_back(iinc);
		}
		else if (tokens[i] == "dec")
		{
			result.program.push_back(idec);
		}
		else if (tokens[i] == "nop")
		{
			result.program.push_back(inop);
		}
		else if (tokens[i] == "print")
		{
			result.program.push_back(iprint);
		}
		else if (tokens[i] == "printstr")
		{
			result.program.push_back(iprintstr);
		}
		else if (tokens[i] == "concat")
		{
			result.program.push_back(iconcat);
		}
		else if (tokens[i] == "store")
		{
			uint64_t address = std::stoull(tokens[++i]);
			result.program.push_back(istore);
			result.program.push_back(address);
		}
		else if (tokens[i] == "pstore")
		{
			uint64_t address = std::stoull(tokens[++i]);
			result.program.push_back(ipstore);
			result.program.push_back(address);
		}
		else if (tokens[i] == "load")
		{
			uint64_t address = std::stoull(tokens[++i]);
			result.program.push_back(iload);
			result.program.push_back(address);
		}
		else if (tokens[i] == "syscall")
		{
			result.program.push_back(isyscall);
		}
		else if (tokens[i] == "jmp")
		{
			int64_t offset = getOffset(tokens, i);
			result.program.push_back(ijmp);
			result.program.push_back(offset);
		}
		else if (tokens[i] == "jz")
		{
			int64_t offset = getOffset(tokens, i);
			result.program.push_back(ijz);
			result.program.push_back(offset);
		}
		else if (tokens[i] == "jnz")
		{
			int64_t offset = getOffset(tokens, i);
			result.program.push_back(ijnz);
			result.program.push_back(offset);
		}
		else if (tokens[i] == "je")
		{
			int64_t offset = getOffset(tokens, i);
			result.program.push_back(ije);
			result.program.push_back(offset);
		}
		else if (tokens[i] == "jne")
		{
			int64_t offset = getOffset(tokens, i);
			result.program.push_back(ijne);
			result.program.push_back(offset);
		}
		else if (tokens[i] == "jgt")
		{
			int64_t offset = getOffset(tokens, i);
			result.program.push_back(ijgt);
			result.program.push_back(offset);
		}
		else if (tokens[i] == "jge")
		{
			int64_t offset = getOffset(tokens, i);
			result.program.push_back(ijge);
			result.program.push_back(offset);
		}
		else if (tokens[i] == "jlt")
		{
			int64_t offset = getOffset(tokens, i);
			result.program.push_back(ijlt);
			result.program.push_back(offset);
		}
		else if (tokens[i] == "gt")
		{
			result.program.push_back(igt);
		}
		else if (tokens[i] == "ge")
		{
			result.program.push_back(ige);
		}
		else if (tokens[i] == "lt")
		{
			result.program.push_back(ilt);
		}
		else if (tokens[i] == "le")
		{
			result.program.push_back(ile);
		}
		else if (tokens[i] == "eq")
		{
			result.program.push_back(ieq);
		}
		else if (tokens[i] == "ne")
		{
			result.program.push_back(ine);
		}
		else if (tokens[i] == "jle")
		{
			int64_t offset = getOffset(tokens, i);
			result.program.push_back(ijle);
			result.program.push_back(offset);
		}
		else if (tokens[i] == "and")
		{
			result.program.push_back(iand);
		}
		else if (tokens[i] == "or")
		{
			result.program.push_back(ior);
		}
		else if (tokens[i] == "dup")
		{
			result.program.push_back(idup);
		}
		else if (tokens[i] == "halt")
		{
			result.program.push_back(ihalt);
		}
		else if (tokens[i] == "call")
		{
			int64_t offset = getOffset(tokens, i);
			result.program.push_back(icall);
			result.program.push_back(offset);
		}
		else if (tokens[i] == "ret")
		{
			result.program.push_back(iret);
		}
		else if (tokens[i] == "retval")
		{
			result.program.push_back(iretval);
		}
		else if (tokens[i] == "tostr")
		{
			result.program.push_back(itostr);
		}
		else if (tokens[i] == "caps")
		{
			result.program.push_back(icaps);			
		}
		else if (tokens[i] == "lower")
		{
			result.program.push_back(ilower);
		}
		else
		{
			result.success = false;
			error("unknown instruction '" + tokens[i] + "'");
			continue;
		}
	}

	return result;
}

bool SVASMParser::isWhitespace(char c) const
{
	return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

bool SVASMParser::isAddress(const std::string &token) const
{
	for (size_t i = 0; i < token.size(); i++)
	{
		if (i == 0 && token[i] == '-')
		{
			continue;
		}

		if (!std::isdigit(token[i]))
		{
			return false;
		}
	}

	return true;
}

int64_t SVASMParser::getOffset(const std::vector<std::string> &tokens, size_t &i) const
{
	if (i + 1 >= tokens.size())
	{
		return 0;
	}

	std::string token = tokens[++i];
	if (isAddress(token))
	{
		return std::stoll(token);
	}

	int64_t address = symbolTable.get(token);
	return address - i - 1;
}

SVASMParserResult SVASMParser::parse(const std::string& source)
{
	std::ifstream file(source);
	if (!file.is_open())
	{
		return parseInternal(source);
	}

	std::string result;
	std::string line;
	while (std::getline(file, line))
	{
		result += line + '\n';
	}

	file.close();
	return parseInternal(result);
}

void SVASMSymbolTable::add(const std::string &name, uint64_t value)
{
	symbols[name] = value;
}

int64_t SVASMSymbolTable::get(const std::string &name) const
{
	auto it = symbols.find(name);
	if (it == symbols.end())
	{
		return -1;
	}

	return it->second;
}

void SVASMSymbolTable::dump() const
{
	for (const auto& symbol : symbols)
	{
		printf("%s: %lu\n", symbol.first.c_str(), symbol.second);
	}
}
