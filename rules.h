#pragma once
#include "predicate.h"
#include <sstream>


using namespace std;

class rules
{
private:
	predicate headPredicate;
	vector <predicate> listOfRules;
public:
	void setHeadPredicate(predicate head)
	{
		headPredicate = head;
	}

	void setRule(vector <predicate> rule)
	{
		for (unsigned int i = 0; i < rule.size(); i++) {
			listOfRules.push_back(rule.at(i));
		}
	}

	predicate getHeadPredicate()
	{
		return headPredicate;
	}

	vector<predicate> getListOfRules() { return listOfRules; }

	void setHeadPredicateParam(parameter newParam)
	{
		headPredicate.addParameter(newParam);
	}

	string toString()
	{
		stringstream ruleSS;
		ruleSS << headPredicate.toString() + " :- ";

		for (unsigned int i = 0; i < listOfRules.size(); i++) 
		{
			if (i == 0) 
			{
				ruleSS << listOfRules.at(i).toString();
			}
			else 
			{
				ruleSS << "," + listOfRules.at(i).toString();
			}
		}

		ruleSS << ".";
		return ruleSS.str();
	}
};
