//
// Created by yael and linoy on 23/12/2019.
//

#ifndef UNTITLED6_VAROBJECTS_H
#define UNTITLED6_VAROBJECTS_H

#include <string>
#include <iostream>
using namespace std;

class VarObject {
public:
    VarObject(string arrow, string sim, float value);
    string getSim();
    float getVarValue();
    string getArrow();
    void setVarValue(float newVal);
private:
    string _sim;
    string _arrow;
    float _varValue;
};

#endif //UNTITLED6_VAROBJECTS_H
