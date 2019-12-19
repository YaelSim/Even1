//
// Created by yael on 19/12/2019.
//
#include "lexer.h"
#include <iostream>
#include <string>
#include <regex>
#include <vector>

using namespace std;

bool Lexer::strWithBrackets(string str) {
    regex check("^(?!$)(?:[^)(]*\\([^()]+\\))*[^)(]*$");
    return regex_match(str, check);
}

bool Lexer::strWithComma(string str) {
    regex check("(.*)(\\,)(.*)");
    return regex_match(str, check);
}

bool Lexer::strWithLeftArrow(string str) {
    regex check("(.*)(<-)(.*)");
    return regex_match(str, check);
}

bool Lexer::strWithRightArrow(string str) {
    regex check("(.*)(->)(.*)");
    return regex_match(str, check);
}

//bool Lexer::strWithVar(string str) {
//    regex check("(.*)(var)(.*)");
//    return regex_match(str, check);
//}

//bool Lexer::strWithQuote(string str) {
//    regex check("\"([^\"]|")(*\\"");
//    return regex_match(str, check);
//}

vector<string> Lexer::lexer(string fileName) {
    string line, token, tempToken, subTempToken, leftBracket = "(", rightBracket = ")", comma = ",", quote = "\"";
    string leftArrow = "<-", rightArrow = "->", space = " ";
    vector<string> lexVec;
    ifstream file;
    file.open(fileName, ios::in);
    if (!file) {
        exit(1);
    }

    // now we will read the file and push into lexer
    while (!file.eof()) {
        getline(file, line);
        if (strWithBrackets(line)) {
            token = line.substr(0, line.find(leftBracket));
            // check if there is an arrow
            if(strWithLeftArrow(token)) {
                tempToken = token.substr(0, token.find(leftArrow));
                // check if there is a space
                if(tempToken.find(space) != -1) {
                    subTempToken = tempToken.substr(0, tempToken.find(space));
                    lexVec.push_back(subTempToken);
                    tempToken = tempToken.substr((tempToken.find(space) + 1));
                    lexVec.push_back(tempToken);
                }
                token = token.substr((token.find(leftArrow) + 2));
                lexVec.push_back(leftArrow);
            }
            if(strWithRightArrow(token)) {
                tempToken = token.substr(0, token.find(rightArrow));
                // check if there is a space
                if(tempToken.find(space) != -1) {
                    subTempToken = tempToken.substr(0, tempToken.find(space));
                    lexVec.push_back(subTempToken);
                    tempToken = tempToken.substr((tempToken.find(space) + 1));
                    lexVec.push_back(tempToken);
                }
                token = token.substr((token.find(rightArrow) + 2));
                lexVec.push_back(rightArrow);
            }
            lexVec.push_back(token);
            token = line.substr((line.find(leftBracket) + 1), line.find(rightBracket));
            token = token.substr(0, token.find(rightBracket));
            if (strWithComma(token)) {
                token = token.substr(0, token.find(comma));
                // should check here if there is " "
                /*if (strWithQuote(token)) {
                    // takes from the first place until "
                    token = token.substr(1, token.find(quote));
                }*/
                lexVec.push_back(token);
                token = line.substr((line.find(comma) + 1));
                token = token.substr(0, token.find(rightBracket));
            }
            lexVec.push_back(token);
        }

    }

    file.close();
    return lexVec;
}

