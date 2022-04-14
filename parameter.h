#pragma once
#include <string>

using namespace std;

class parameter 
{
private:
    string thisParameter;
    bool isID = true;
public:
    parameter() = default;
    parameter(string newParam) : thisParameter(newParam) {}
    void setParam(string param)
    {
        thisParameter = param;  
    }

    string getParameter() { return thisParameter; }

    void setIsID(bool type) { isID = type; }

    bool getIsID() { return isID; }

    string toString()
    {
        return thisParameter;
    }
};
