//
// Created by yael on 19/12/2019.
//

#ifndef UNTITLED6_SYMBOLTABLE_H
#define UNTITLED6_SYMBOLTABLE_H

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "Command.h"
using namespace std;

class SymbolTable {
private:
    unordered_map<string,double> namesAndValuesMap;
    unordered_map<string,pair<string,string>> namesAndPairsMap;

public:
    //SymbolTable()
};

#endif //UNTITLED6_SYMBOLTABLE_H
