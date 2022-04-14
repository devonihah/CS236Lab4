#pragma once

#include <string>
#include <map>

using namespace std;

enum tokenType {
	COMMA, PERIOD, LEFT_PAREN, RIGHT_PAREN, Q_MARK, ADD, MULTIPLY, COLON, COLON_DASH,
	STRING, SCHEMES, QUERIES, RULES, FACTS, ID, COMMENT, UNDEFINED, END
};

class token
{
private:
	tokenType type;
	string value;
	int line;
	map<tokenType, string> mapOfTokens;
public:
	token(tokenType t, string v, int l) :type(t), value(v), line(l)
	{
		mapOfTokens[COMMA] = "COMMA";
		mapOfTokens[PERIOD] = "PERIOD";
		mapOfTokens[LEFT_PAREN] = "LEFT_PAREN";
		mapOfTokens[RIGHT_PAREN] = "RIGHT_PAREN";
		mapOfTokens[Q_MARK] = "Q_MARK";
		mapOfTokens[ADD] = "ADD";
		mapOfTokens[MULTIPLY] = "MULTIPLY";
		mapOfTokens[COLON] = "COLON";
		mapOfTokens[COLON_DASH] = "COLON_DASH";
		mapOfTokens[STRING] = "STRING";
		mapOfTokens[SCHEMES] = "SCHEMES";
		mapOfTokens[QUERIES] = "QUERIES";
		mapOfTokens[RULES] = "RULES";
		mapOfTokens[FACTS] = "FACTS";
		mapOfTokens[ID] = "ID";
		mapOfTokens[COMMENT] = "COMMENT";
		mapOfTokens[UNDEFINED] = "UNDEFINED";
		mapOfTokens[END] = "EOF";
	}

	~token()
	{
	}

	tokenType getType()
	{
		return type;
	}

	string getValue()
	{
		return value;
	}

	string getTypeString()
	{
		return mapOfTokens[type];
	}

	string toString();

	string lineToString();
};
