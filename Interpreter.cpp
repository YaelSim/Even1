//
// Created by linoy on 11/11/2019.
//

#include <regex>
#include <queue>
#include <stack>
#include <string>
#include "Interpreter.h"
#include "ex1.h"

using namespace std;

Expression *Interpreter::interpret(string expStr) {
    string token;
    bool isItFirst = true;
    Expression* finalRes;
    size_t pos, len = 1, operFound, matchFound = 0;
    for (pos = 0; pos < expStr.length(); pos++) {
        matchFound = 0;
        token = expStr.substr(pos, len);
        //Check if one of the operators is within the token
        operFound = token.find('+');
        if (operFound != string::npos) {
            checkForComponents(token, operFound, isItFirst, expStr, pos);
            len = 1;
            matchFound++;
        }
        operFound = token.find('-');
        if (operFound != string::npos) {
            checkForComponents(token, operFound, isItFirst, expStr, pos);
            len = 1;
            matchFound++;
        }
        operFound = token.find('/');
        if (operFound != string::npos) {
            checkForComponents(token, operFound, isItFirst, expStr, pos);
            len = 1;
            matchFound++;
        }
        operFound = token.find('*');
        if (operFound != string::npos) {
            checkForComponents(token, operFound, isItFirst, expStr, pos);
            len = 1;
            matchFound++;
        }
        operFound = token.find('(');
        if (operFound != string::npos) {
            checkForComponents(token, operFound, isItFirst, expStr, pos);
            len = 1;
            matchFound++;
        }
        operFound = token.find(')');
        if (operFound != string::npos) {
            checkForComponents(token, operFound, isItFirst, expStr, pos);
            len = 1;
            matchFound++;
        } if (matchFound == 0) {
            pos--;
            len++;
        } else if (token.length() > 1) {
            pos += token.length() - 1;
        } if (matchFound != 0) {
            isItFirst = false;
        } if ((pos + len) > expStr.size()) {
            checkLastGivenValue(token);
            break;
        }
    }
    finalRes = shuntingYard();
    return finalRes;
}

Interpreter::Interpreter() {
    _varQueue = new queue<string>;
    _operDeque = new deque<char>;
}

Interpreter::~Interpreter() {
    delete(_varQueue);
    _varQueue = nullptr;
    delete(_operDeque);
    _operDeque = nullptr;
    vector<string>().swap(_varsDefinitions);
}

void Interpreter::setVariables(string strVars) {
    string token, varVal;
    const char* exception = "Error! Illegal variable assignment!";
    size_t found;
    int nameWellDefined = 0, valWellDefined = 0;
    char semicolon = ';', delimEquals = '=';
    stringstream wholeStream(strVars);
    while (getline(wholeStream, token, semicolon)) {
        valWellDefined = 0;
        nameWellDefined = 0;
        if (token.empty()) {
            break;
        }
        // look for pattern "x = number"
        found = token.find(delimEquals);
        if (found != string::npos) {
            stringstream varStream(token);
            while (getline(varStream, varVal, delimEquals)) {
                if (varVal.empty()) {
                    throw exception;
                } else if (isItNumPattern(varVal)) {
                    valWellDefined++;
                } else if (isItVarNamePattern(varVal)) {
                    nameWellDefined++;
                }
                if ((valWellDefined == 1) && (nameWellDefined == 1)) {
                    _varsDefinitions.push_back(token);
                } else if ((valWellDefined == 0) && (nameWellDefined == 0)) {
                    throw exception;
                }
            }
            if (((valWellDefined == 1) && (nameWellDefined == 0)) ||
                ((valWellDefined == 0) && (nameWellDefined == 1))) {
                throw exception;
            }
        } else {
            throw exception;
        }
    }

}

void Interpreter::checkForComponents(string token, size_t found, bool isFirst, string expStr, size_t currPos) {
    size_t begin = 0;
    const char* exception = "BAD input - doesn't meet the requirements!";
    bool isUnary;
    string potentialName, varNameCheck;
    if (token[found] == ')') {
        //Check if a left bracket exists in deque.
        if (find(_operDeque->begin(), _operDeque->end(), '(') == _operDeque->end()) {
            throw exception;
        } else {
            if (token.length() > 1) {
                potentialName = token.substr(begin, (token.length() - 1));
                if (isItVarNamePattern(potentialName)) {
                    matchQueueAndDefinitions(potentialName);
                } else if (isItNumPattern(potentialName)) {
                    _varQueue->push(token.substr(begin, (token.length() - 1)));
                } else {
                    throw exception;
                }
            }
            //If ')' was found, move all operators in the deque until the first '(' to the queue of nums.
            while (_operDeque->back()) {
                if (_operDeque->back() == '(') {
                    break;
                } else {
                    string poppedOperator(1, _operDeque->back());
                    _varQueue->push(poppedOperator);
                    _operDeque->pop_back();
                }
            }
            //Pop also the '('. If we have an unary operator that is related to this expression, pop it too.
            _operDeque->pop_back();
            if ((!_operDeque->empty()) && ((_operDeque->back() == '#') || (_operDeque->back() == '$'))) {
                string poppedUnary(1, _operDeque->back());
                _varQueue->push(poppedUnary);
                _operDeque->pop_back();
            }
        }
    } else {
        isUnary = isItUnary(token, found, isFirst);
        if (isUnary) {
            //Uplus is represented as '#', Uminus is '$'
            // -3*x is OK, but -x*3 isn't. Read the next char. If it's a var- Exception. Else - It's valid.
            //varNameCheck = expStr.substr(expStr.find(token) + token.length(), 1);
            varNameCheck = expStr.substr(currPos + token.length(), 1);
            if (isItVarNamePattern(varNameCheck)) {
                throw exception;
            }
            if (token[found] == '+') {
                _operDeque->insert(_operDeque->end(), '#');
            } else if (token[found] == '-') {
                _operDeque->insert(_operDeque->end(), '$');
            }
        } else {
            _operDeque->insert(_operDeque->end(), token[found]);
        }
        //Check if there's also a variable/number BESIDES the operator
        if (token.length() > 1) {
            potentialName = token.substr(begin, (token.length() - 1));
            if (isItVarNamePattern(potentialName)) {
                matchQueueAndDefinitions(potentialName);
            } else if (isItNumPattern(potentialName)) {
                _varQueue->push(token.substr(begin, (token.length() - 1)));
            } else {
                throw exception;
            }
        }
    }
}

Expression* Interpreter::shuntingYard() {
    stack<double> calcStack;
    const char* exception = "Error! Illegal math expression!";
    while (!_operDeque->empty()) {
        string poppedOperator(1, _operDeque->back());
        _varQueue->push(poppedOperator);
        _operDeque->pop_back();
    }
    while (!_varQueue->empty()) {
        string popOfQueue = _varQueue->front();
        if (isItNumPattern(popOfQueue)) {
            calcStack.push(stod(popOfQueue));
            _varQueue->pop();
        } else if ((popOfQueue == "+") || (popOfQueue == "-") || (popOfQueue == "*") || (popOfQueue == "/")) {
            if (calcStack.size() < 2) {
                throw exception;
            }  else {
                Value* rightVal = new Value(calcStack.top());
                calcStack.pop();
                Value* leftVal = new Value(calcStack.top());
                calcStack.pop();
                if (popOfQueue == "+") {
                    Expression* result = new Plus(leftVal, rightVal);
                    calcStack.push(result->calculate());
                    if (result != nullptr) {
                        delete result;
                        result = nullptr;
                    }
                } else if (popOfQueue == "-") {
                    Expression* result = new Minus(rightVal, leftVal);
                    calcStack.push(result->calculate());
                    if (result != nullptr) {
                        delete result;
                        result = nullptr;
                    }
                } else if (popOfQueue == "*") {
                    Expression* result = new Mul(leftVal, rightVal);
                    calcStack.push(result->calculate());
                    if (result != nullptr) {
                        delete result;
                        result = nullptr;
                    }
                } else {
                    Expression* result = new Div(leftVal, rightVal);
                    calcStack.push(result->calculate());
                    if (result != nullptr) {
                        delete result;
                        result = nullptr;
                    }
                }
                _varQueue->pop();
            }
            //Uplus - # ; Uminus - $
        } else if ((popOfQueue == "#") || (popOfQueue == "$")) {
            Value* operand = new Value(calcStack.top());
            calcStack.pop();
            if (popOfQueue == "#") {
                Expression* result = new UPlus(operand);
                calcStack.push(result->calculate());
                if (result != nullptr) {
                    delete result;
                    result = nullptr;
                }
            } else {
                Expression* result = new UMinus(operand);
                calcStack.push(result->calculate());
                if (result != nullptr) {
                    delete result;
                    result = nullptr;
                }
            }
            _varQueue->pop();
        }
    }
    if (calcStack.size() != 1) {
        throw exception;
    } else {
        Expression* finalResult = new Value(calcStack.top());
        return finalResult;
    }
}

void Interpreter::matchQueueAndDefinitions(string varName) {
    vector<string> varVals;
    int flag = 0;
    const char* exception = "Variables don't match with definition of setVariables!";
    size_t found, comparisonWithInput;
    string delimEquals = "=";
    for (size_t i = 0; i < _varsDefinitions.size(); i++) {
        found = _varsDefinitions[i].find(varName);
        if (found != string::npos) {
            comparisonWithInput = _varsDefinitions[i].find(delimEquals);
            if (comparisonWithInput != string::npos) {
                if (string(&(_varsDefinitions[i][found]), &_varsDefinitions[i][comparisonWithInput])
                    == varName) {
                    if (_varsDefinitions[i][_varsDefinitions[i].length() - 1] == ';') {
                        varVals.push_back(_varsDefinitions[i].substr(found + (varName.length() + 1),
                                                                     (_varsDefinitions[i].length() - 2 - varName.length())));
                    } else {
                        varVals.push_back(_varsDefinitions[i].substr(found + (varName.length() + 1),
                                                                     _varsDefinitions[i].length() - 1 - varName.length()));
                    }
                    flag++;
                }
            }
        }
    }
    if ((varVals.empty()) || (flag == 0)) {
        throw exception;
    } else {
        _varQueue->push(varVals.back());
    }
}

bool Interpreter::isItUnary(string token, size_t operFound, bool isFirst) {
    if (operFound == 0) {
        if (((!_operDeque->empty()) && (_operDeque->back() == '(')) || ((_operDeque->empty()) && (isFirst))) {
            string charBeforeOp;
            if (token[operFound] == '+' || token[operFound] == '-') {
                return true;
            }
        }
    } else {
        string charBeforeOp(1, token[operFound - 1]);
        if (token[operFound] == '+' || token[operFound] == '-') {
            if (charBeforeOp.empty() || charBeforeOp == "(") {
                return true;
            }
        }
    }
    return false;
}

bool Interpreter::isItNumPattern(string potential) {
    regex numPattern("^[-+]?[0-9]+\\.?[0-9]*$");
    return regex_match(potential, numPattern);
}

bool Interpreter::isItVarNamePattern(string potential) {
    regex varNamePattern("^(?![0-9])([a-zA-Z]|_|[0-9])+");
    return regex_match(potential, varNamePattern);
}

void Interpreter::checkLastGivenValue(string potential) {
    const char* exception = "Error!";
    if (isItNumPattern(potential)) {
        _varQueue->push(potential);
    } else if (isItVarNamePattern(potential)) {
        matchQueueAndDefinitions(potential);
    } else {
        throw exception;
    }
}