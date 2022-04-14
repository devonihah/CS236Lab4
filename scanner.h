#pragma once

#include "token.h"
#include <fstream>
#include <vector>

using namespace std;

class scanner
{
private:
	ifstream inputFile;
	ofstream outputFile;
	char currentChar = ' ';
	char nextChar = ' ';
	string value = "";
	string input = "";
	int line = 1;
	int lineStart = 0;
	vector <token> tokenVector;
	//token tokenType;

public:
	scanner() {}
	scanner(string argv)
	{
		outputFile << argv;
	}
	~scanner() {}
	vector <token> scan(string nameOfFile);
	void tokenScanner();
	void stringScanner();
	void IDScanner();
	void commentScanner();
	void tokenCreator(tokenType type, int lineNum);
	vector <token> getTokenVector()
	{
		return tokenVector;
	}
	string toString();

};
