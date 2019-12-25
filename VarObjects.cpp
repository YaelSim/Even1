//
// Created by linoy on 22/12/2019.
//

#include "VarObjects.h"

VarObject::VarObject(string arrow, string sim, double value) {
    this->_arrow = arrow;
    this->_sim = sim;
    this->_varValue = value;
}

string VarObject::getArrow() {
    return this->_arrow;
}

double VarObject::getVarValue() {
    return this->_varValue;
}

string VarObject::getSim() {
    return this->_sim;
}

void VarObject::setVarValue(double val) {
    this->_varValue = val;
}



