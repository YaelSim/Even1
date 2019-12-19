//
// Created by yael on 16/12/2019.
//
#include "lexer.h"
#include <string>
#include <cstring>

using namespace std;

//vector<string>  lexer(ifstream file) {
//
//}


int main(int argc, char** argv) {
    Lexer lex;
    string fileName = argv[1];
    vector<string> vecLexer;
    vecLexer = lex.lexer(fileName);


    return 0;
}