//
// Created by yael and linoy on 19/12/2019.
//

#ifndef UNTITLED6_PARSER_H
#define UNTITLED6_PARSER_H


#include <string>
#include <iostream>
#include <vector>
#include <map>
#include "Command.h"
#include "SymbolTable.h"
#include "VarObjects.h"

using namespace std;

class Parser {
public:
    Parser() = default;
    map<string, Command*> createCommandMap();
    void parser(vector<string> lexVec);
};
#endif //UNTITLED6_PARSER_H
