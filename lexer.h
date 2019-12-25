//
// Created by yael on 19/12/2019.
//

#ifndef EVEN1_LEXER_H
#define EVEN1_LEXER_H


#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
using namespace std;
class Lexer {
private:
    bool strWithBrackets(string str);
    bool strWithComma(string str);
    bool strWithRightArrow(string str);
    bool strWithLeftArrow(string str);
    bool strWithEqual(string str);
    bool strWithWhile(string str);
    bool strWithIf(string str);
    bool strWithVar(string str);
public:
    Lexer() = default;
    vector<string> lexer(string fileName);
};


#endif //EVEN1_LEXER_H
