//
// Created by yael on 19/12/2019.
//

#ifndef UNTITLED6_LEXER_H
#define UNTITLED6_LEXER_H

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
//    bool strWithVar(string str);
//    bool strWithQuote(string str);
public:
    Lexer() = default;
    vector<string> lexer(string fileName);
};

#endif //UNTITLED6_LEXER_H
