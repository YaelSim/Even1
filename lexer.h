//
// Created by yael on 19/12/2019.
//

#ifndef UNTITLED6_LEXER_H
#define UNTITLED6_LEXER_H

#include <iostream>
#include <fstream>
#include <string>
#include <bits/locale_facets.tcc>
#include <bits/regex.h>
#include <vector>
using namespace std;
class Lexer {
public:
    bool strWithPar(string str);
    vector<string> lexer(string fileName);
};

#endif //UNTITLED6_LEXER_H
