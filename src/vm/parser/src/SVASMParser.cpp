#include <fstream>

#include "../include/SVASMParser.h"

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
            if (!token.empty())
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
				result.program.push_back(std::stoull(tokens[++i]));
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
		else if (tokens[i] == "fact")
		{
			result.program.push_back(ifact);
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
		else if (tokens[i] == "halt")
		{
			result.program.push_back(ihalt);
		}
		else
		{
			result.success = false;
			printf("error: unknown instruction '%s'\n", tokens[i].c_str());
			return result;
		}
	}

	return result;
}

bool SVASMParser::isWhitespace(char c) const
{
	return c == ' ' || c == '\t' || c == '\n' || c == '\r';
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
