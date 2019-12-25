//
// Created by linoy on 25/12/2019.
//

#ifndef EVEN1_EXPRESSION_H
#define EVEN1_EXPRESSION_H

using namespace std;
/**
 * Expression Interface
 */
class Expression {

public:
    virtual double calculate() = 0;
    virtual ~Expression() {}
};

#endif //EVEN1_EXPRESSION_H
