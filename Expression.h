//
// Created by yael on 10/11/2019.
//


#ifndef UNTITLED6_EXPRESSION_H
#define UNTITLED6_EXPRESSION_H

using namespace std;
/**
 * Expression Interface
 */
class Expression {

public:
    virtual double calculate() = 0;
    virtual ~Expression() {}
};

#endif //PROJECT_EXPRESSION_H
