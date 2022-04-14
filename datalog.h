#pragma once
#include <vector>
#include <set>
#include "rules.h"

enum predicateType{ Schemes, Queries, Facts };

class datalog
{
private:
	vector <rules> rulesVector;
	vector <predicate> schemesVector;
	vector <predicate> queriesVector;
	vector <predicate> factsVector;
	set <string> domain;

public:
	void setRules(vector <rules> newRules)
	{
		for (unsigned int i = 0; i < newRules.size(); i++)
		{
			rulesVector.push_back(newRules.at(i));
		}
	}

	void setQueries(vector<predicate> newQueries)
	{
		for (unsigned int i = 0; i < newQueries.size(); i++)
		{
			queriesVector.push_back(newQueries.at(i));
		}
	}

	void setSchemes(vector<predicate> newSchemes)
	{
		for (unsigned int i = 0; i < newSchemes.size(); i++)
		{
			schemesVector.push_back(newSchemes.at(i));
		}
	}

	void setFacts(vector <predicate> newFacts)
	{
		for (unsigned int i = 0; i < newFacts.size(); i++)
		{
			factsVector.push_back(newFacts.at(i));
		}
	}

	void setDomain(set<string> newDomain) { domain = newDomain; }

	vector<rules> getRules() { return rulesVector; }

	vector<predicate> getQueries() { return queriesVector; }

	vector<predicate> getFacts() { return factsVector; }

	vector<string> getFactsString()
	{
		vector<string> factsVectorString;
		for (size_t i = 0; i < factsVector.size(); i++)
		{
			factsVectorString.at(i) = factsVector.at(i).toString();
		}
		return factsVectorString;
	}

	vector<predicate> getSchemes() { return schemesVector; }

	string toString()
	{
		stringstream datalogSS;

		datalogSS << "Schemes(" << schemesVector.size() << "):\n";
		for (unsigned int i = 0; i < schemesVector.size(); i++) {
			datalogSS << "  " << schemesVector.at(i).toString() << "\n";
		}

		datalogSS << "Facts(" << factsVector.size() << "):\n";
		for (unsigned int i = 0; i < factsVector.size(); i++) {
			datalogSS <<"  " << factsVector.at(i).toString() << ".\n";
		}

		datalogSS << "Rules(" << rulesVector.size() << "):\n";
		for (unsigned int i = 0; i < rulesVector.size(); i++) {
			datalogSS << "  " << rulesVector.at(i).toString() << "\n";
		}

		datalogSS << "Queries(" << queriesVector.size() << "):\n";
		for (unsigned int i = 0; i < queriesVector.size(); i++) {
			datalogSS << "  " << queriesVector.at(i).toString() << "?\n";
		}

		datalogSS << "Domain(" << domain.size() << "):";
		set<string>::iterator it;
		for (it = domain.begin(); it != domain.end(); it++) {
			datalogSS << "\n" << "  " << *it;
		}

		return datalogSS.str();
	}

};
