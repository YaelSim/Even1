//
// Created by yael and linoy on 22/12/2019.
//

#include "VarObjects.h"

//This is a data class - it holds the data members that each variable should have.
VarObject::VarObject(string arrow, string sim, float value) {
    this->_arrow = arrow;
    this->_sim = sim;
    this->_varValue = value;
}

// a getter of the arrow
string VarObject::getArrow() {
    return this->_arrow;
}

// a getter of the value
float VarObject::getVarValue() {
    return this->_varValue;
}

// a getter of the sim path
string VarObject::getSim() {
    return this->_sim;
}

// a setter of the value
void VarObject::setVarValue(float val) {
    this->_varValue = val;
}



