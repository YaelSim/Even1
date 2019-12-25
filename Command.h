//
// Created by yael on 19/12/2019.
//
#ifndef EVEN1_COMMAND_H
#define EVEN1_COMMAND_H

#include <string>
#include <sstream>
#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <mutex>
#include <unistd.h>
#include <vector>
#include "VarObjects.h"
#include "SymbolTable.h"
#include <regex>
#include "ex1.h"
#include "Expression.h"
#include "Interpreter.h"

using namespace std;

int readWithServer(string portToListen);

class Command {
protected:
    int convertStringToInt(string toConvert);
    bool isItVarNamePattern(string potential);
    bool isItExpressionPattern(string potential);
public:
    virtual int execute(vector<string> lexer) = 0;
};

class OpenServerCommand : public Command {
public:
    OpenServerCommand() = default;
    int execute(vector<string> lexer) override;
};

class UpdateVarCommand : public Command {
public:
    UpdateVarCommand() = default;
    int execute(vector<string> lexer) override;
};

class DefineVarCommand : public Command {
public:
    DefineVarCommand() = default;
    int execute(vector<string> lexer) override;
};

class ConnectCommand : public Command {
public:
    ConnectCommand() = default;
    int execute(vector<string> lexer) override;
};

class PrintCommand : public Command {
public:
    PrintCommand() = default;
    int execute(vector<string> lexer) override;
};

class ConditionParser : public Command {
protected:
    double getParametersValue(string param);
    map<string, Command*> commandsForLoops(vector<string> lexer);
    bool isConditionTrue(double param1, double param2, string condition);
public:
    ConditionParser() = default;
    int execute(vector<string> lexer) override = 0;
};

class WhileCommand : public ConditionParser {
public:
    WhileCommand() = default;
    int execute(vector<string> lexer) override;
};

class IfCommand : public ConditionParser {
public:
    IfCommand() = default;
    int execute(vector<string> lexer) override;
};

class SleepCommand : public Command {
public:
    SleepCommand() = default;
    int execute(vector<string> lexer) override;
};
#endif //EVEN1_COMMAND_H