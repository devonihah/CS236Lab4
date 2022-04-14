#pragma once
#include "scheme.h"
#include <vector>
#include <string>
#include <sstream>

using namespace std;

class tuples
{
private:
	vector<string> values;
public:
	tuples() = default;
	tuples(vector<string> newValues) : values(newValues) {}
	string at(int index) { return values.at(index); }
	void push_back(string newValue) { values.push_back(newValue);  }
	size_t size() { return values.size(); }
	void setvalues(vector<string> newValues) { values = newValues; }
	vector<string> getValues() { return values; }
	bool operator<(const tuples& newTuple) const { return this->values < newTuple.values; }
	
	string toString(scheme customScheme) const
	{
		stringstream out;

		for (unsigned int i = 0; i < values.size(); i++)
		{
			if (i > 0)
			{
				out << ", ";
			}
			string name = customScheme.getAttributes().at(i);
			string value = values.at(i);
			out << name << "=" << value;
		}

		return out.str();
	}
	
};
