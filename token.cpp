

#include "token.h"
#include <string>
#include <sstream>

using namespace std;

string token::toString()
{
	string out;
	out = "(" + mapOfTokens[type] + "," + value + "," + lineToString() + ")";
	return out;
}

string token::lineToString()
{
	stringstream ss;
	ss << line;
	string lineNum = ss.str();

	return lineNum;
}
