#pragma once
#include <string>
#include <vector>
#include <map>
#include "relation.h"
#include "tuple.h"
using namespace std;

class database {
private:

public:
	map<string, relation> relations;
	database() = default;
	void addRelation(relation newRelation) { relations[newRelation.getID()] = newRelation; }
    void addRelationMap(string name, relation newRelation)
    {
        relations.insert({ name, newRelation });
    }
	void addTupleToRelation(string id, tuples newTuple) { relations[id].addTuple(newTuple); }
	relation findRelation(string relationName) { return relations[relationName]; }
    string toString()
    {
        stringstream stringy;
        map<string, relation> ptr;
        for (auto i : relations) { // Problems out putting look here and check with project 3
            cout << i.first << ": \n";
            i.second.toString();
        }
        return stringy.str();
    }
};
