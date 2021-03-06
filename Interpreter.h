//
// Created by linoy on 12/11/2019.
//

#include <regex>
#include <queue>
#include <stack>
#include <iostream>
#include "Expression.h"

#ifndef UNTITLED6_INTERPRETER_H
#define UNTITLED6_INTERPRETER_H

using namespace std;

class Interpreter {
    queue <string>* _varQueue;
    deque <char>* _operDeque;
    vector<string> _varsDefinitions;

    void checkForComponents(string token, size_t found, bool isFirst, string expStr, size_t currPos);

    bool isItUnary(string token, size_t operIndex, bool isFirst);

    void checkLastGivenValue(string potential);

    bool isItNumPattern(string potential);

    bool isItVarNamePattern(string potential);

    Expression* shuntingYard();

    void matchQueueAndDefinitions(string varName);

public:
    Expression* interpret(string expStr);

    void setVariables(string strVars);

    Interpreter();

    ~Interpreter();
};

#endif //UNTITLED1_INTERPRETER_H
