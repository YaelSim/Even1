//
// Created by linoy on 10/11/2019.
//
#include "Expression.h"
#include "Interpreter.h"

using namespace std;

#ifndef UNTITLED6_EX1_H
#define UNTITLED6_EX1_H

class Value : public Expression {
private:
    double _value;

public:
    Value(double val);

    double getValue();

    virtual ~Value();

    double calculate();
};

class Variable : public Expression {
private:
    string _varName;
    double _value;

public:
    Variable(string name, double varVal);

    virtual ~Variable();

    Variable& operator++(int);

    Variable& operator--(int);

    Variable& operator+=(double val);

    Variable& operator-=(double val);

    Variable& operator++();

    Variable& operator--();

    double calculate();
};

class BinaryOperator : public Expression {
protected:
    Expression* _rightExp;

    Expression* _leftExp;

public:
    BinaryOperator(Expression* right, Expression* left);

    virtual ~BinaryOperator();
};

class Plus : public BinaryOperator {
public:
    Plus(Expression *right, Expression *left);

    virtual ~Plus();

    double calculate();
};

class Minus : public BinaryOperator {
public:
    Minus(Expression *right, Expression *left);

    virtual ~Minus();

    double calculate();
};

class Mul : public BinaryOperator {
public:
    Mul(Expression *right, Expression *left);

    double calculate();

    virtual ~Mul();
};

class Div : public BinaryOperator {
public:
    Div(Expression *right, Expression *left);

    double calculate();

    virtual ~Div();
};

class UnaryOperator : public Expression {
protected:
    Expression* _exp;

public:
    UnaryOperator(Expression* expression);

    virtual ~UnaryOperator();
};

class UPlus : public UnaryOperator {
public:
    UPlus(Expression* expression);

    double calculate();

    virtual ~UPlus();
};

class UMinus : public UnaryOperator {
public:
    UMinus(Expression* expression);

    double calculate();

    virtual ~UMinus();
};

#endif //UNTITLED1_EX1_H