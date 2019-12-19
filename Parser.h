//
// Created by yael on 19/12/2019.
//

#ifndef UNTITLED6_PARSER_H
#define UNTITLED6_PARSER_H

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "Command.h"
using namespace std;

class Parser {
public:
    Parser() = default;
    unordered_map<string, Command> createCommandMap(vector<string> lexVec);
    unordered_map<string, double> createSymbolTableMap(vector<string> lexVec);
    void parser(vector<string> lexVec, unordered_map<string, Command> commandMap);

};

#endif //UNTITLED6_PARSER_H
