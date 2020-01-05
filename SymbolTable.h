//
// Created by yael and linoy on 19/12/2019.
//

#ifndef UNTITLED6_SYMBOLTABLE_H
#define UNTITLED6_SYMBOLTABLE_H

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "VarObjects.h"

using namespace std;

class SymbolTable {
private:
    //txt Map is for "->"; xml Map is for "<-"
    unordered_map<string,VarObject*> txtSymbolTable;
    unordered_map<string,VarObject*> xmlSymbolTable;
public:
    SymbolTable() = default;
    void actOnTxtSymbolTable(string name, VarObject* varObj);
    void actOnXmlSymbolTable(string name, VarObject* varObj);
    bool findInTxtMap(string name);
    bool findInXmlMap(string name);
    unordered_map<string, VarObject*> getTxtSymbolTable();
    unordered_map<string, VarObject*> getXmlSymbolTable();
};

#endif //UNTITLED6_SYMBOLTABLE_H
