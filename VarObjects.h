//
// Created by linoy on 22/12/2019.
//

#ifndef EVEN1_VAROBJECTS_H
#define EVEN1_VAROBJECTS_H
#include <string>
#include <iostream>
using namespace std;

class VarObject {
public:
    VarObject(string arrow, string sim, double value);
    string getSim();
    double getVarValue();
    string getArrow();
    void setVarValue(double newVal);
private:
    string _sim;
    string _arrow;
    double _varValue;
};

#endif //EVEN1_VAROBJECTS_H
