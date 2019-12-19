//
// Created by yael on 19/12/2019.
//

#ifndef UNTITLED6_COMMAND_H
#define UNTITLED6_COMMAND_H

#include <string>
#include <iostream>
using namespace std;

class Command {
public:
    virtual int execute() = 0;
};

class OpenServerCommand : public Command {
public:
    int execute() override;
};

class DefineVarCommand : public Command {
private:
    string _varName;
    string _arrow;
    string _simParam;
    double _varValue = 0;
public:
    int execute() override;
};

class ConnectCommand : public Command {
public:
    int execute() override;
};

class PrintCommand : public Command {
private:
    string _toPrint;
public:
    int execute() override;
};

class WhileCommand : public Command {
private:
    string _condition;

public:
    int execute() override;
};

class SleepCommand : public Command {
private:
    int _secsToSleep = 0;
public:
    int execute() override;
};
#endif //UNTITLED6_COMMAND_H
