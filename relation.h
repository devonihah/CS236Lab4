#pragma once
#include <vector>
#include <string>
#include <set>
#include <list>
#include "scheme.h"
#include "tuple.h"

using namespace std;

class relation {

private:

    string id;
    scheme schemes;
    set<tuples> tuplesSet;

public:
    relation() = default;
    relation(string name, scheme scheme) : id(name), schemes(scheme) { }
    string getID() { return id; }
    void setID(string newID) { id = newID; }
    scheme getScheme() { return schemes; }
    void setScheme(scheme newScheme) { schemes = newScheme; }
    void addSchemeAttribute(string newAttribute)
    {
        schemes.Push_back(newAttribute);
    }
    void addTuple(const tuples& tupleToAdd) { tuplesSet.insert(tupleToAdd); }
    void setTuples(set<tuples> tuples) { tuplesSet = tuples; }
    unsigned int tuplesSetSize() 
    {
        unsigned int tuplesSetSize = (unsigned int)tuplesSet.size();
        return tuplesSetSize;
    }

    relation select(int index1, int index2)
    {
        relation newRelation(id, schemes);
        for (tuples it : tuplesSet)
        {
            if (it.getValues()[index1] == it.getValues()[index2])
            {
                newRelation.addTuple(it);
            }
        }
        return newRelation;
    }

    relation select(int index, string value) const {
        relation newRelation(id, schemes);
        for (auto i : tuplesSet)
        {
            if (i.at(index) == value)
            {
                newRelation.addTuple(i);
            }
        }
        return newRelation;
    }

    relation project(vector<int> indices)
    {
        vector<string> newAttributes;
        for (int it : indices)
        {
            newAttributes.push_back(schemes.getAttributes()[it]);
        }
        scheme newScheme(newAttributes);
        //cout << "Num of Attributes: " << newScheme.size() << endl;
        relation newRelation(id, newScheme);

        for (tuples it : tuplesSet)
        {
            vector<string> newValues;
            for (int ti : indices)
            {
                newValues.push_back(it.getValues()[ti]);
            }
            tuples newTuple(newValues);
            newRelation.addTuple(newTuple);
        }
        return newRelation;
    }

    relation rename(vector<string> newAttributes)
    {
        scheme newScheme(newAttributes);
        relation newRelation(id, newScheme);
        newRelation.setTuples(tuplesSet);
        return newRelation;
    }

    bool unionRelation(relation relationToUnion)
    {
        bool returnValue = false;
        for (auto i : relationToUnion.tuplesSet)
        {
            ////cout << "New Relation tuples values: " << endl;
            //for (auto it : tuplesSet)
            //{
            //    for (unsigned int j = 0; j < it.getValues().size(); j++)
            //    {
            //        //cout << it.getValues().at(j) << endl;
            //    }
            //}
            ////cout << "Old Relation tuples values: " << endl;
            //for (auto m : relationToUnion.tuplesSet)
            //{
            //    for (unsigned int j = 0; j < i.getValues().size(); j++)
            //    {
            //        //cout << i.getValues().at(j) << endl;
            //    }
            //}
            if (tuplesSet.insert(i).second)
            {
                returnValue = true;
                //cout << endl << "Returning TRUE" << endl << endl;
                if (schemes.size() != 0) { cout << "  "; }
                for (unsigned int j = 0; j < schemes.size(); j++)
                {
                    cout << schemes.at(j) + "=" + i.at(j);
                    if (j < schemes.size() - 1)
                    {
                        cout << ", ";
                    }
                }
                if (schemes.size() != 0) { cout << endl; }
            }
            //else { cout << "RETURNING FALSE" << endl; }
        }
        return returnValue;
    }

    bool joinable(scheme scheme1, scheme scheme2, tuples tuple1, tuples tuple2) 
    {
        //cout << scheme1.size() << endl;
        //cout << scheme2.size() << endl;
        for (unsigned int i = 0; i < scheme1.size(); i++)
        {
            for (unsigned int j = 0; j < scheme2.size(); j++)
            {
                if (scheme1.at(i) == scheme2.at(j))
                {
                    if (tuple1.at(i) != tuple2.at(j)) 
                    {
                        //cout << "false" << endl;
                        return false; 
                    }
                }
            }
        }
        //cout << "true" << endl;
        return true;
    }

    relation join(relation relationToCheck) {
        relation newRelation;
        vector <int> relations;
        vector <int> intsToAdd;
        newRelation.setScheme(schemes);

        for (unsigned int i = 0; i < relationToCheck.schemes.size(); i++)
        {
            bool relationIsDuplicate = false;
            for (unsigned int j = 0; j < schemes.size(); j++) 
            {
                if (!relationIsDuplicate)
                {
                    if (schemes.at(j) != relationToCheck.getScheme().at(i)) { relationIsDuplicate = false; }
                    else { relationIsDuplicate = true; }
                }
            }
            if (!relationIsDuplicate)
            {
                
                newRelation.addSchemeAttribute(relationToCheck.getScheme().at(i));
                //cout << "Scheme Size: " << newRelation.getScheme().size() << endl;
                //cout << relationToCheck.getScheme().at(i) << endl;
                intsToAdd.push_back(i);
            }
        }

        for (auto i : tuplesSet)
        {
            for (auto j : relationToCheck.tuplesSet)
            {
                if (joinable(schemes, relationToCheck.schemes, i, j))
                {
                    tuples newTuple = i;
                    for (unsigned int k = 0; k < intsToAdd.size(); k++)
                    {
                        newTuple.push_back(j.at(intsToAdd.at(k)));
                    }
                    newRelation.addTuple(newTuple);
                }
            }
        }
        return newRelation;
    }
    
    string toString() {
        stringstream ss;
        for (auto i : tuplesSet) {
            if (schemes.size() != 0) {
                cout << "  ";
            }
            for (unsigned int j = 0; j < schemes.size(); j++) {
                cout << schemes.at(j) + "=" + i.at(j);
                if (j < schemes.size() - 1) {
                    cout << ", ";
                }
            }
            if (schemes.size() != 0) {
                cout << "\n";
            }
        }
        return ss.str();
    }
    string rString;
    /*
    for (auto i : DemTuples) {
        if (TheScheme.Size() != 0) {
            cout << "  ";
        }
        for (unsigned int j = 0; j < TheScheme.Size(); j++) {
            cout << TheScheme.At(j) + "=" + i.at(j);
            if (j < TheScheme.Size() - 1) {
                cout << ", ";
            }
        }
        if (TheScheme.Size() != 0) {
            cout << "\n";
        }
    }
    return rString;
    */
    
};
