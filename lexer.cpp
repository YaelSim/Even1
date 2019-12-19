//
// Created by yael on 19/12/2019.
//
#include "lexer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <bits/locale_facets.tcc>
#include <regex>
#include <vector>

using namespace std;

bool Lexer::strWithPar(string str) {
    regex check(R"((\().*(\)))");
    return regex_match(str, check);
}

vector<string> Lexer::lexer(string fileName) {
    string word, token;
    vector<string> lexersArr;
    ifstream file;
    file.open(fileName, ios::in);
    if (!file) {
        exit(1);
    }

    // now we will read the file and push into lexer
    while(!file.eof()) {
    file >> word;
    cout << word << endl;

    }

    file.close();
    return lexersArr;
}
