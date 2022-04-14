#pragma once
#include "parameter.h"
#include <vector>
#include <sstream>

using namespace std;

class predicate
{
private:
	vector <parameter> listOfParameters;
	string predicateName;
public:
	vector<parameter> getVector()
	{
		return listOfParameters;
	}

	void setPredicateName(string predName)
	{
		predicateName = predName;
	}

	string getPredicateName() { return predicateName; }

	void addParameter(parameter newParameter)
	{
		listOfParameters.push_back(newParameter);
	}

	vector<parameter> getParams() { return listOfParameters; }

	vector<string> getParamsString()
	{
		vector<string> paramsStringVector;
		for (size_t i = 0; i < listOfParameters.size(); i++)
		{
			paramsStringVector.push_back(listOfParameters.at(i).toString());
		}
		return paramsStringVector;
	}

	string toString()
	{
		stringstream paramString;
		paramString << predicateName + "(";

		for (unsigned int i = 0; i < listOfParameters.size(); i++) 
		{
			if (i != 0)
			{
				paramString << ",";
			}
			paramString << listOfParameters.at(i).toString();
		}

		paramString << ")";
		return paramString.str();
	}
};
