//
// Created by yael on 19/12/2019.
//

#ifndef EVEN1_SYMBOLTABLE_H
#define EVEN1_SYMBOLTABLE_H

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "Command.h"
#include "VarObjects.h"

using namespace std;

class SymbolTable {
private:
    //unordered_map<string,double> namesAndValuesMap;
    //unordered_map<string,pair<string,string>> namesAndPairsMap;
    //the first string in the pair is the "arrow", the second string in the pair is the "sim"
    unordered_map<string,VarObject> txtSymbolTable;
    unordered_map<string,VarObject> xmlSymbolTable;
    //txt Map is for "->"; xml Map is for "<-"
public:
    SymbolTable() = default;
    void actOnTxtSymbolTable(string name, VarObject varObj);
    void actOnXmlSymbolTable(string name, VarObject varObj);
    bool findInTxtMap(string name);
    bool findInXmlMap(string name);
    unordered_map<string, VarObject> getTxtSymbolTable();
    unordered_map<string, VarObject> getXmlSymbolTable();
};

#endif //EVEN1_SYMBOLTABLE_H
