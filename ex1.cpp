//
// Created by linoy on 10/11/2019.
//

#include <iostream>
#include "Expression.h"
#include "ex1.h"

using namespace std;

Value::Value(double val) {
    _value = val;
}

Value::~Value() = default;

double Value::calculate() {
    return _value;
}

double Value::getValue() {
    return _value;
}

Variable::Variable(string name, double varVal) {
    _varName = name;
    _value = varVal;
}

Variable &Variable::operator++(int) {
    _value += 1;
    return *this;
}

Variable &Variable::operator--(int) {
    _value -= 1;
    return *this;
}

Variable &Variable::operator+=(double val) {
    _value += val;
    return *this;
}

Variable &Variable::operator-=(double val) {
    _value -= val;
    return *this;
}

Variable &Variable::operator++() {
    _value += 1;
    return *this;
}

Variable &Variable::operator--() {
    _value -= 1;
    return *this;
}

double Variable::calculate() {
    return _value;
}

Variable::~Variable() = default;

BinaryOperator::BinaryOperator(Expression *right, Expression *left) {
    _rightExp = right;
    _leftExp = left;
}

BinaryOperator::~BinaryOperator() {
    if (_rightExp != nullptr) {
        delete _rightExp;
        _rightExp = nullptr;
    } if (_leftExp != nullptr) {
        delete _leftExp;
        _leftExp = nullptr;
    }
}

Plus::Plus(Expression *right, Expression *left) : BinaryOperator(right, left) {
    _rightExp = right;
    _leftExp = left;
}

double Plus::calculate() {
    double rightVal = _rightExp->calculate();
    double leftVal = _leftExp->calculate();
    return (rightVal + leftVal);
}

Plus::~Plus() {
    if (_rightExp != nullptr) {
        delete _rightExp;
        _rightExp = nullptr;
    } if (_leftExp != nullptr) {
        delete _leftExp;
        _leftExp = nullptr;
    }
}

Minus::Minus(Expression *right, Expression *left) : BinaryOperator(right, left) {
    _rightExp = right;
    _leftExp = left;
}

double Minus::calculate() {
    double rightVal = _rightExp->calculate();
    double leftVal = _leftExp->calculate();
    return (leftVal - rightVal);
}

Minus::~Minus() {
    if (_rightExp != nullptr) {
        delete _rightExp;
        _rightExp = nullptr;
    } if (_leftExp != nullptr) {
        delete _leftExp;
        _leftExp = nullptr;
    }
}

Mul::Mul(Expression *right, Expression *left) : BinaryOperator(right, left) {
    _rightExp = right;
    _leftExp = left;
}

double Mul::calculate() {
    double rightVal = _rightExp->calculate();
    double leftVal = _leftExp->calculate();
    return (leftVal * rightVal);
}

Mul::~Mul() {
    if (_rightExp != nullptr) {
        delete _rightExp;
        _rightExp = nullptr;
    } if (_leftExp != nullptr) {
        delete _leftExp;
        _leftExp = nullptr;
    }
}

Div::Div(Expression *right, Expression *left) : BinaryOperator(right, left) {
    _rightExp = right;
    _leftExp = left;
}

double Div::calculate() {
    double rightVal = _rightExp->calculate();
    double leftVal = _leftExp->calculate();
    if (leftVal == 0) {
        const char* exception = "Error! Division by 0 is forbidden!";
        throw exception;
    }
    return (rightVal / leftVal);
}

Div::~Div() {
    if (_rightExp != nullptr) {
        delete _rightExp;
        _rightExp = nullptr;
    } if (_leftExp != nullptr) {
        delete _leftExp;
        _leftExp = nullptr;
    }
}

UnaryOperator::UnaryOperator(Expression *expression) {
    _exp = expression;
}

UnaryOperator::~UnaryOperator() {
    if (_exp != nullptr) {
        delete _exp;
        _exp = nullptr;
    }
}

UPlus::UPlus(Expression *expression) : UnaryOperator(expression) {
    _exp = expression;
}

double UPlus::calculate() {
    return _exp->calculate();
}

UPlus::~UPlus() {
    if (_exp != nullptr) {
        delete _exp;
        _exp = nullptr;
    }
}

UMinus::UMinus(Expression *expression) : UnaryOperator(expression) {
    _exp = expression;
}

double UMinus::calculate() {
    return ((-1) * (_exp->calculate()));
}

UMinus::~UMinus() {
    if (_exp != nullptr) {
        delete _exp;
        _exp = nullptr;
    }
}