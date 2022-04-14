#pragma once

#include "token.h"
#include "scanner.h"
#include "datalog.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

class parser
{
private:
	vector <token> parserTokens;
	vector <rules> ruleVector;
	vector <predicate> rulePredicateVector;
	vector <predicate> schemeVector;
	vector <predicate> factVector;
	vector <predicate> queryVector;
	vector <token> errorVector;
	datalog fullDatalog;
	set <string> domain;

public:
	parser(const vector<token>& tokens) : parserTokens(tokens) { }

	datalog getDatalog() { return fullDatalog; }

	string parseTokens()
	{
		while (parserTokens.size() > 0 && errorVector.size() < 1)
		{
			//check what the token type is and add the respective types to their vectors
			if (getTokenType() == SCHEMES)
			{
				//cout << "SCHEMES\n";
				match(SCHEMES);
				match(COLON);
				while (getTokenType() != FACTS && errorVector.size() < 1) { scheme(); }
				if (schemeVector.size() <= 0) throwError();
			}
			if (getTokenType() == FACTS)
			{
				//cout << "FACTS\n";
				match(FACTS);
				match(COLON);
				while (getTokenType() != RULES && errorVector.size() < 1) { fact(); }
				vector<predicate> newFactsVector;
				bool duplicateFact = false;
				for (unsigned int i = 0; i < factVector.size() - 1; i++)
				{
					duplicateFact = false;
					for (unsigned int j = i + 1; j < factVector.size(); j++)
					{
						if (factVector.at(i).getPredicateName() == factVector.at(j).getPredicateName() && factVector.at(i).getParamsString() == factVector.at(j).getParamsString()) duplicateFact = true;
					}
					if (!duplicateFact) newFactsVector.push_back(factVector.at(i));
				}
				newFactsVector.push_back(factVector.at(factVector.size() - 1));
				factVector = newFactsVector;
			}
			if (getTokenType() == RULES)
			{
				//cout << "RULES\n";
				match(RULES);
				match(COLON);
				while (getTokenType() != QUERIES && errorVector.size() < 1)
				{
					rule();
					ruleVector.at(ruleVector.size() - 1).setRule(rulePredicateVector);
					rulePredicateVector.clear();
				}
			}
			if (getTokenType() == QUERIES)
			{
				//cout << "QUERIES\n";
				match(QUERIES);
				match(COLON);
				while (getTokenType() != END && errorVector.size() < 1) { query(); }
				if (queryVector.size() <= 0) throwError();
				match(END);
			}
			else
			{
				throwError();
			}
		}
		if (errorVector.size() > 0)
		{
			stringstream out;
			cout << "Failure!\n  " << errorVector.at(0).toString();
			return out.str();
		}
		else
		{
			stringstream out;
			//cout << "Success!" << endl;
			setDatalog();
			//cout << printDatalog();
			//cout << endl;
			//cout << schemeVector.size() << endl;
			//cout << factVector.size() << endl;
			//cout << ruleVector.size() << endl;
			//cout << queryVector.size() << endl;
			return out.str();
		}
	}

	void setDatalog()
	{
		fullDatalog.setQueries(queryVector);
		fullDatalog.setSchemes(schemeVector);
		fullDatalog.setFacts(factVector);
		fullDatalog.setRules(ruleVector);
		fullDatalog.setDomain(domain);
	}

	string printDatalog()
	{
		return fullDatalog.toString();
	}

	tokenType getTokenType() //get the current token type from parseTokens vector
	{
		if (parserTokens.size() == 0)
		{
			return UNDEFINED;
		}
		return parserTokens.at(0).getType();
	}

	void advanceToken() //get rid of current token and advance to next one
	{
		parserTokens.erase(parserTokens.begin());
		//cout << parserTokens.size() << endl;
	}

	void throwError() //throw an error if the syntax does not match up correctly
	{
		//cout << parserTokens.at(0).toString() << endl;
		errorVector.push_back(parserTokens.at(0));
	}

	void match(tokenType t) //check if the token type passed in matches the current token in parseTokens vector
	{
		//cout << "match: " << t << endl;
		if (getTokenType() == t)
		{
			advanceToken();
		}
		//else if (getTokenType() == COMMENT)
		//{
		//	match(t);
		//}
		else
			throwError();
	}

	void scheme() //create a predicate that has a name and parameters
	{
		if (getTokenType() == ID)
		{
			while (getTokenType() == ID)
			{
				//this section creates the name of the predicate and the parameters of the predicates
				predicate schemePredicate;
				//cout << parserTokens.at(0).getValue() << endl;
				schemePredicate.setPredicateName(parserTokens.at(0).getValue());
				schemeVector.push_back(schemePredicate);
				match(ID);
				match(LEFT_PAREN);
				parameter schemeParameter;
				//cout << parserTokens.at(0).getValue() << endl;
				schemeParameter.setParam(parserTokens.at(0).getValue());
				//cout << schemeParameter.toString();
				schemeVector.at(schemeVector.size() - 1).addParameter(schemeParameter);
				//cout << schemeVector.at(0).getParamsString().size();
				match(ID);
				schemeList();
				match(RIGHT_PAREN);
				if (schemeVector.at(schemeVector.size() - 1).getParams().size() <= 0) throwError();
			}
		}
		else
		{
			throwError();
		}
	}

	void schemeList()
	{
		if (getTokenType() == RIGHT_PAREN) { return; } //if token type is the right parenthese then scheme is done
		if (getTokenType() == COMMA)
		{
			match(COMMA);

			parameter schemeParameter;
			//cout << parserTokens.at(0).getValue() << endl;
			schemeParameter.setParam(parserTokens.at(0).getValue());
			schemeVector.at(schemeVector.size() - 1).addParameter(schemeParameter);
			match(ID);

			schemeList(); //get next parameter
		}
		else
		{
			throwError();
		}
	}

	void fact() //create a predicate that has a name and parameters
	{
		if (getTokenType() == ID)
		{
			while (getTokenType() == ID)
			{
				//this section creates the name of the predicate and the parameters of the predicates
				predicate factPredicate;
				//cout << parserTokens.at(0).getValue() << endl;
				factPredicate.setPredicateName(parserTokens.at(0).getValue());
				factVector.push_back(factPredicate);
				match(ID);
				match(LEFT_PAREN);
				parameter factParameter;
				//cout << parserTokens.at(0).getValue() << endl;
				domain.insert(parserTokens.at(0).getValue());
				factParameter.setParam(parserTokens.at(0).getValue());
				factVector.at(factVector.size() - 1).addParameter(factParameter);
				//if (getTokenType() == STRING) { 
				match(STRING);
				//}
				factList();
				match(RIGHT_PAREN);
				match(PERIOD);
				if (factVector.at(factVector.size() - 1).getParams().size() <= 0) throwError();
			}
		}
		else
		{
			throwError();
		}
	}

	void factList() //if token type is the right parenthese then scheme is done
	{
		if (getTokenType() == RIGHT_PAREN) { return; }
		if (getTokenType() == COMMA)
		{
			match(COMMA);

			parameter factParameter;
			//cout << parserTokens.at(0).getValue() << endl;
			domain.insert(parserTokens.at(0).getValue());
			factParameter.setParam(parserTokens.at(0).getValue());
			//if (getTokenType() == ID)
			//{
			//	factParameter.setIsID(true);
			//	factVector.at(factVector.size() - 1).addParameter(factParameter);
			//	match(ID);
			//}
			if (getTokenType() == STRING)
			{
				factParameter.setIsID(false);
				factVector.at(factVector.size() - 1).addParameter(factParameter);
				match(STRING);
			}
			factList();  //get next parameter
		}
		else
		{
			throwError();
		}
	}

	void rule()
	{
		if (getTokenType() == ID)
		{
			predicate rulePredicate;
			rules headPredicate;
			//cout << parserTokens.at(0).getValue() << endl;
			rulePredicate.setPredicateName(parserTokens.at(0).getValue());
			headPredicate.setHeadPredicate(rulePredicate); //set the head predicate for the rule
			ruleVector.push_back(headPredicate);
			match(ID);
			match(LEFT_PAREN);
			parameter headPredicateParameter;
			//cout << parserTokens.at(0).getValue() << endl;
			headPredicateParameter.setParam(parserTokens.at(0).getValue());
			ruleVector.at(ruleVector.size() - 1).setHeadPredicateParam(headPredicateParameter); //set the first parameter for the head predicate
			if (getTokenType() == ID) { match(ID); }
			else if (getTokenType() == STRING) { match(STRING); }
			ruleHeadPredicate(); //set the rest of the parameters for the head predicate

			match(RIGHT_PAREN);
			match(COLON_DASH);
			if (getTokenType() == COMMA) { throwError(); }
			do //get the rest of the predicates that establish the rule
			{
				if (getTokenType() == COMMA) { match(COMMA); }
				//cout << parserTokens.at(0).getValue() << endl;
				rulePredicate.setPredicateName(parserTokens.at(0).getValue());
				rulePredicateVector.push_back(rulePredicate);
				if (getTokenType() == ID) { match(ID); }
				else if (getTokenType() == STRING) { match(STRING); }
				match(LEFT_PAREN);
				parameter ruleParameter;
				//cout << parserTokens.at(0).getValue() << endl;
				ruleParameter.setParam(parserTokens.at(0).getValue());
				rulePredicateVector.at(rulePredicateVector.size() - 1).addParameter(ruleParameter);
				if (getTokenType() == ID) { match(ID); }
				else if (getTokenType() == STRING) { match(STRING); }
				ruleList();
				match(RIGHT_PAREN);
				for (unsigned int i = 0; i < rulePredicateVector.size(); i++)
				{
					//cout << rulePredicateVector.at(i).toString() << endl;
				}
			} while (getTokenType() == COMMA);

			match(PERIOD);
		}
		else
		{
			throwError();
		}
	}

	void ruleHeadPredicate()
	{
		if (getTokenType() == RIGHT_PAREN) { return; }
		if (getTokenType() == COMMA)
		{
			match(COMMA);

			parameter ruleParameter;
			//cout << parserTokens.at(0).getValue() << endl;
			ruleParameter.setParam(parserTokens.at(0).getValue());
			ruleVector.at(ruleVector.size() - 1).setHeadPredicateParam(ruleParameter);
			match(ID);
			ruleHeadPredicate();
		}
		else
		{
			throwError();
		}
	}

	void ruleList()
	{
		if (getTokenType() == RIGHT_PAREN) { return; }
		if (getTokenType() == COMMA)
		{
			match(COMMA);
			if (getTokenType() == ID || getTokenType() == STRING)
			{
				parameter ruleParameter;
				//cout << parserTokens.at(0).getValue() << endl;
				ruleParameter.setParam(parserTokens.at(0).getValue());
				rulePredicateVector.at(rulePredicateVector.size() - 1).addParameter(ruleParameter);
				if (getTokenType() == ID) { match(ID); }
				else if (getTokenType() == STRING) { match(STRING); }
			}
			ruleList();
		}
		else
		{
			throwError();
		}
	}

	void query()
	{
		if (getTokenType() == ID)
		{
			while (getTokenType() == ID)
			{
				//this section creates the name of the predicate and the parameters of the predicates
				predicate queryPredicate;
				//cout << parserTokens.at(0).getValue() << endl;
				queryPredicate.setPredicateName(parserTokens.at(0).getValue());
				queryVector.push_back(queryPredicate);
				match(ID);
				match(LEFT_PAREN);
				parameter queryParameter;
				//cout << parserTokens.at(0).getValue() << endl;
				queryParameter.setParam(parserTokens.at(0).getValue());
				if (getTokenType() == ID)
				{
					queryParameter.setIsID(true);
					queryVector.at(queryVector.size() - 1).addParameter(queryParameter);
					match(ID);
				}
				else if (getTokenType() == STRING)
				{
					queryParameter.setIsID(false);
					queryVector.at(queryVector.size() - 1).addParameter(queryParameter);
					match(STRING);
				}
				else { throwError(); }
				queryList();
				match(RIGHT_PAREN);
				match(Q_MARK);
			}
		}
		else
		{
			throwError();
		}
	}

	void queryList()
	{
		if (getTokenType() == RIGHT_PAREN) { return; }
		if (getTokenType() == COMMA)
		{
			match(COMMA);
			parameter queryParameter;
			//cout << parserTokens.at(0).getValue() << endl;
			queryParameter.setParam(parserTokens.at(0).getValue());
			if (getTokenType() == ID)
			{
				queryParameter.setIsID(true);
				queryVector.at(queryVector.size() - 1).addParameter(queryParameter);
				match(ID);
			}
			else if (getTokenType() == STRING)
			{
				queryParameter.setIsID(false);
				queryVector.at(queryVector.size() - 1).addParameter(queryParameter);
				match(STRING);
			}
			else { throwError(); }
			queryList();
		}
		else
		{
			throwError();
		}
	}
};

