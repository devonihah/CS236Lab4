

#include "scanner.h"
#include <fstream>
#include <cctype>
#include <iostream>
#include <sstream>

using namespace std;

vector <token> scanner::scan(string nameOfFile)
{
	inputFile.open(nameOfFile.c_str());

	if (inputFile)
	{
		while (inputFile.get(currentChar))
		{
			tokenScanner();
		}

		tokenCreator(END, line);

		//cout << "Total Tokens = " << tokenVector.size() << endl;

		//outputFile.close();
	}

	else
	{
		//cout << "File did not load" << endl;
	}

	return tokenVector;
}


void scanner::tokenScanner()
{
	while (isspace(currentChar))
	{
		if (currentChar == '\n')
		{
			line++;
		}
		inputFile.get(currentChar);
		if (inputFile.eof())
		{
			break;
		}
	}

	switch (currentChar)
	{
	case ',':
		value = ',';
		tokenCreator(COMMA, line);
		break;

	case '.':
		value = '.';
		tokenCreator(PERIOD, line);
		break;

	case '(':
		value = "(";
		tokenCreator(LEFT_PAREN, line);
		break;

	case ')':
		value = ")";
		tokenCreator(RIGHT_PAREN, line);
		break;

	case '?':
		value = '?';
		tokenCreator(Q_MARK, line);
		break;

	case '+':
		value = "+";
		tokenCreator(ADD, line);
		break;

	case '*':
		value = "*";
		tokenCreator(MULTIPLY, line);
		break;

	case ':':
		value = ":";
		nextChar = inputFile.peek();
		if (nextChar == '-')
		{
			value = ":-";
			tokenCreator(COLON_DASH, line);
			inputFile.get(currentChar);
		}
		else
		{
			tokenCreator(COLON, line);
		}
		break;

	case '\'':
		stringScanner();
		break;

	case '#':
		commentScanner();
		break;

	default:
		if (isalpha(currentChar))
		{
			IDScanner();
		}

		else if (!inputFile.eof())
		{
			value += currentChar;
			tokenCreator(UNDEFINED, line);
		}

		break;
	}
}

void scanner::stringScanner()
{
	bool stringEnd = false;
	value += currentChar;
	lineStart = line;

	while (!stringEnd)
	{
		if (inputFile.peek() == -1)
		{
			tokenCreator(UNDEFINED, lineStart);
			stringEnd = true;
		}
		else
		{
			inputFile.get(currentChar);
			switch (currentChar)
			{
			case '\n':
				line++;
				value += currentChar;
				break;
			case '\'':
				value += currentChar;
				nextChar = inputFile.peek();
				if (nextChar != '\'')
				{
					tokenCreator(STRING, lineStart);
					stringEnd = true;
				}
				else
				{
					inputFile.get(currentChar);
					value += currentChar;
				}
				break;
			default:
				value += currentChar;
				break;
			}

		}

	}
}

void scanner::commentScanner()
{
	value += currentChar;
	lineStart = line;
	nextChar = inputFile.peek();

	if (nextChar != '|')
	{
		inputFile.get(currentChar);
		while (currentChar != '\n')
		{
			value += currentChar;
			inputFile.get(currentChar);
		}

		if (currentChar == '\n')
		{
			line++;
		}
		tokenCreator(COMMENT, lineStart);
	}
	else
	{
		inputFile.get(currentChar);
		value += currentChar;
		inputFile.get(currentChar);
		nextChar = inputFile.peek();

		while ((currentChar != '|' || nextChar != '#') && nextChar != -1)
		{
			if (currentChar == '\n')
			{
				line++;
			}
			value += currentChar;
			inputFile.get(currentChar);
			nextChar = inputFile.peek();
		}

		if (currentChar == '|')
		{
			if (nextChar == '#')
			{
				value = value + currentChar;
				inputFile.get(currentChar);
				value = value + currentChar;
				tokenCreator(COMMENT, lineStart);
			}
		}
		else if (inputFile.eof())
		{
			line++;
			tokenCreator(UNDEFINED, lineStart);
		}
	}
}

void scanner::IDScanner()
{
	value += currentChar;
	inputFile.get(currentChar);

	while (isalnum(currentChar))
	{
		value += currentChar;
		inputFile.get(currentChar);
	}


	if (value == "Schemes")
	{
		tokenCreator(SCHEMES, line);
	}
	else if (value == "Queries")
	{
		tokenCreator(QUERIES, line);
	}
	else if (value == "Rules")
	{
		tokenCreator(RULES, line);
	}
	else if (value == "Facts")
	{
		tokenCreator(FACTS, line);
	}
	else
	{
		tokenCreator(ID, line);
	}
	tokenScanner();
}

void scanner::tokenCreator(tokenType type, int lineNum)
{
	if (type != COMMENT)
	{
		token myToken(type, value, lineNum);
		tokenVector.push_back(myToken);
	}
	//outputFile << myToken.toString() << endl;
	//cout << myToken.toString() << endl;
	value = "";
}

string scanner::toString()
{
	stringstream ss;
	for (unsigned int i = 0; i < tokenVector.size(); i++)
	{
		ss << tokenVector.at(i).toString() << endl;
		//cout << tokenVector.at(i).toString() << endl;
	}

	ss << "Total Tokens = " << tokenVector.size();

	return ss.str();
}