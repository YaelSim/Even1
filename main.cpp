//
// Created by yael on 16/12/2019.
//
#include "lexer.h"
#include "Parser.h"
#include <string>

using namespace std;

int main(int argc, char** argv) {
    Lexer lex;
    string fileName = argv[1];
    vector<string> vecLexer;
    vecLexer = lex.lexer(fileName);
    Parser parser;
    parser.parser(vecLexer);

    return 0;
}