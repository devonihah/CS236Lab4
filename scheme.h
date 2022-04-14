#pragma once
#include <vector>
#include <string>
#include <sstream>
using namespace std;

class scheme
{
private:
	vector<string> attributes;
public:
	scheme() = default;
	scheme(vector<string> names) : attributes(names) {}
	void addAttribute(string newAttribute) { attributes.push_back(newAttribute); }
	void setAttributes(vector<string> newAttributes) { attributes = newAttributes; }
	void Push_back(string newAttribute)
	{
		attributes.push_back(newAttribute);
		for (unsigned int i = 0; i < attributes.size(); i++)
		{
			//cout << attributes.at(i) << endl;
		}
		//cout << "Num of Attributes: " << size() << endl;
	}
	vector<string> getAttributes() { return attributes; }
	size_t size() { return getAttributes().size(); }
	string at(int index) { return getAttributes().at(index); }
	string toString()
	{
		stringstream out;
		out << attributes.size() << endl;
		for (unsigned int i = 0; i < attributes.size(); i++)
		{
			out << attributes.at(i) << endl;
		}
		return out.str();
	}
};
