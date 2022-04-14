#pragma once

#include <set>
#include <vector>
#include "rules.h"
#include "database.h"
#include "datalog.h"
#include "predicate.h"
#include "relation.h"
#include <iostream>


class interpreter {
private:
	database myDatabase;
	datalog myDatalog;
public:
	interpreter(datalog newDatalog)
	{
		myDatalog = newDatalog;
		SchemesToDatabase(myDatalog.getSchemes());
		FactsToDatabase(myDatalog.getFacts());
		//cout << "CHECKING WHAT IS IN THE DATABASE" << endl;
		//myDatabase.toString();
		//cout << "ALL DONE CHECKING" << endl << endl;
		interpretRules(myDatalog.getRules());
		interpretQueries();
	}	

	void SchemesToDatabase(vector<predicate> newPredicates) {
		for (unsigned int i = 0; i < newPredicates.size(); i++) {
			string theName = newPredicates.at(i).getPredicateName();
			scheme theScheme;
			for (unsigned int j = 0; j < newPredicates.at(i).getParams().size(); j++) {
				theScheme.Push_back(newPredicates.at(i).getParams().at(j).toString());
			}
			//relation newRelation(theName, theScheme);
			relation newRelation;
			newRelation.setID(theName);
			newRelation.setScheme(theScheme);
			myDatabase.addRelationMap(theName, newRelation);	
		}
		return;
	}

	void FactsToDatabase(vector<predicate> Facts) {
		for (unsigned int j = 0; j < Facts.size(); j++) {
			string theName;
			theName = Facts.at(j).getPredicateName();
			tuples theTuple;
			for (unsigned int i = 0; i < Facts.at(j).getParams().size(); i++) {
				theTuple.push_back(Facts.at(j).getParams().at(i).toString());
			}
			myDatabase.addTupleToRelation(theName, theTuple);
		}
		return;
	}

	void interpretQueries()
	{
		//FIX THIS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		cout << "Query Evaluation" << endl;
		vector<predicate> datalogQueries = myDatalog.getQueries();
		for (unsigned int i = 0; i < datalogQueries.size(); i++)
		{
			relation relationsToGrab, columnsToGrab;
			relationsToGrab = myDatabase.findRelation(datalogQueries[i].getPredicateName());
			vector<int> Positions;
			vector<string> renameVals;

			for (unsigned int j = 0; j < datalogQueries[i].getVector().size(); j++)
			{
				//int nextParameterToSelect = j + 1;
				parameter nextParameterToCheck = datalogQueries[i].getVector()[j];
				if (!nextParameterToCheck.getIsID())
					relationsToGrab = relationsToGrab.select(j+1, nextParameterToCheck.toString());
			}

			for (unsigned int j = 0; j < datalogQueries[i].getVector().size(); j++)
			{
				for (unsigned int k = j + 1; k < datalogQueries[i].getVector().size(); k++)
				{
					bool exitLoop = false;
					if (datalogQueries[i].getVector()[j].toString() == datalogQueries[i].getVector()[k].toString())
					{
						relationsToGrab = relationsToGrab.select(j, k);
						exitLoop = true;
					}
					if (exitLoop) break;
				}
			}

			for (unsigned int j = 0; j < datalogQueries[i].getVector().size(); j++)
			{
				parameter nextParameterToCheck = datalogQueries[i].getVector()[j];
				if (nextParameterToCheck.getIsID())
				{
					bool duplicate = false;
					for (unsigned int x = 0; x < Positions.size(); x++)
					{
						if (nextParameterToCheck.toString() == renameVals[x])
						{
							duplicate = true;
						}
						if (duplicate)
						{
							break;
						}
					}
					if (!duplicate)
					{
						Positions.push_back(j);
						renameVals.push_back(nextParameterToCheck.toString());
					}

				}
			}

			columnsToGrab = relationsToGrab.project(Positions);

			scheme nextScheme;
			nextScheme.setAttributes(renameVals);
			columnsToGrab.setScheme(nextScheme);

			cout << datalogQueries[i].toString() << "? ";
			if (relationsToGrab.tuplesSetSize() == 0)
			{
				cout << "No\n";
			}
			else
			{
				cout << "Yes(" << columnsToGrab.tuplesSetSize() << ")\n";
				cout << columnsToGrab.toString();
			}
		}
	}

	relation interpretPredicate(predicate newPredicate)
	{
		string theName = newPredicate.getPredicateName();
		vector<int> theInts;
		vector<string> theStrings;
		relation thisRelation = myDatabase.findRelation(theName);
		for (unsigned int j = 0; j < newPredicate.getVector().size(); j++) {
			parameter currentParameter = newPredicate.getVector().at(j);
			if (currentParameter.getIsID() == false) {
				string yes = currentParameter.getParameter();
				//cout << yes << endl;
				thisRelation = thisRelation.select(j, yes);
			}
			else {
				bool duplicate = false;
				for (unsigned int k = 0; k < theStrings.size(); k++) {
					if (theStrings.at(k) == currentParameter.getParameter()) {
						duplicate = true;
						thisRelation = thisRelation.select(j, k);
					}
				}
				if (duplicate == false) {
					theStrings.push_back(currentParameter.getParameter());
					theInts.push_back(j);
				}
			}
		}
		thisRelation = thisRelation.project(theInts);
		thisRelation = thisRelation.rename(theStrings);
		//thisRelation.toString();
		return thisRelation;
	}

	void interpretRules(vector<rules> newRules)
	{
		cout << "Rule Evaluation" << endl;
		bool moreTuplesToCreate = true;
		int numOfIterations = 0;
		while (moreTuplesToCreate)
		{
			//size_t numOfRelations = myDatabase.relations.size();
			moreTuplesToCreate = false;
			vector <bool> ruleContinuesIterating;
			numOfIterations++;
			for (unsigned int i = 0; i < newRules.size(); i++)
			{
				cout << newRules.at(i).toString() << endl;
				vector <relation> newRelations;
				for (unsigned int j = 0; j < newRules.at(i).getListOfRules().size(); j++)
				{
					relation thisRelation = interpretPredicate(newRules.at(i).getListOfRules().at(j));
					newRelations.push_back(thisRelation);
				}

				relation thisRelation;
				if (newRelations.size() > 1)
				{
					thisRelation = newRelations.at(0);
					for (unsigned int j = 0; j < newRelations.size() - 1; j++)
					{
						thisRelation = thisRelation.join(newRelations.at(j + 1));
					}
				}

				vector<int> indices;
				//cout << thisRelation.getScheme().size() << endl;
				for (unsigned int j = 0; j < newRules.at(i).getHeadPredicate().getParams().size(); j++)
				{
					for (unsigned int k = 0; k < thisRelation.getScheme().size(); k++)
					{
						//cout << newRules.at(i).getHeadPredicate().getParams().at(j).getParameter() << endl;
						//cout << thisRelation.getScheme().at(k) << endl;

						if (newRules.at(i).getHeadPredicate().getParams().at(j).getParameter() == thisRelation.getScheme().at(k))
						{
							indices.push_back(k);
						}
					}
				}
				thisRelation = thisRelation.project(indices);
				thisRelation.setID(newRules.at(i).getHeadPredicate().getPredicateName());
				if (myDatabase.relations.at(thisRelation.getID()).getScheme().getAttributes().size() ==
					thisRelation.getScheme().getAttributes().size())
				{
					thisRelation.setScheme(myDatabase.relations.at(thisRelation.getID()).getScheme());
				}
				//cout << "PRINTING OUT THE STRING:" << endl;
				//thisRelation.toString();
				//cout << endl;
				//myDatabase.relations.at(thisRelation.getID()).toString();
				ruleContinuesIterating.push_back(myDatabase.relations.at(thisRelation.getID()).unionRelation(thisRelation));
			}
			for (unsigned int m = 0; m < ruleContinuesIterating.size(); m++)
			{
				if (ruleContinuesIterating.at(m)) { moreTuplesToCreate = true; }
				//cout << m << ": " << ruleContinuesIterating.at(m) << endl;
			}
		}
		cout << endl << "Schemes populated after " << numOfIterations << " passes through the Rules." << endl << endl;
		return;
	}
};
