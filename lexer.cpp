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
    regex check("(.*)\\((.*)\\)(.*)");
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


bool Lexer::strWithEqual(string str) {
    bool result;
    //regex check("(.*)(=)(.*)");
    regex check("(.*)[^<>](=)(.*)");
    return regex_match(str, check);
}

bool Lexer::strWithWhile(string str) {
    regex check("(.*)(while)(.*)");
    return regex_match(str, check);
}

bool Lexer::strWithVar(string str) {
    regex check("(.*)(var)(.*)");
    return regex_match(str, check);
}

bool Lexer::strWithIf(string str) {
    regex check("(.*)(if)(.*)");
    return regex_match(str, check);
}

//bool Lexer::strWithQuote(string str) {
//    regex check("\"([^\"]|")(*\\"");
//    return regex_match(str, check);
//}

vector<string> Lexer::lexer(string fileName) {
    string line, token, tempToken, subTempToken, leftBracket = "(", rightBracket = ")", comma = ",", quote = "\"";
    string leftArrow = "<-", rightArrow = "->", space = " ", equal = "=";
    int flag, flagWhile;
    vector<string> lexVec;
    ifstream file;
    file.open(fileName, ios::in);
    if (!file) {
        cout << "can't open the file - no arguments" << endl;
        exit(1);
    }

    // now we will read the file and push into lexer
    while (!file.eof()) {
        flag = 0;
        flagWhile = 0;
        getline(file, line);
        if(strWithWhile(line) || strWithIf(line)) {
            // do until the end of the while
            while(line != "}") {
                // check if there is =
                if(strWithEqual(line)) {
                    token = line.substr(0, line.find(equal));
                    // remove the space before
                    if(token.find("\t") != -1) {
                        token = token.substr((line.find("\t") + 1));
                    }
                    lexVec.push_back(token);
                    lexVec.push_back(equal);
                    line = line.substr((line.find(equal) + 2));
                    lexVec.push_back(line);
                    // go to the next line
                    getline(file, line);
                    continue;
                }

                while (line.find(space) != -1) {
                    tempToken = line.substr(0, line.find(space));
                    if(tempToken.find(leftBracket) != -1) {
                        subTempToken = tempToken.substr(0, tempToken.find(leftBracket));
                        lexVec.push_back(subTempToken);
                        tempToken = tempToken.substr((tempToken.find(leftBracket) + 1));
                        // check inside brackets
                        if(tempToken.find(leftBracket) != -1) {
                            tempToken = line.substr(line.find(leftBracket) + 2);
                        }
                        tempToken = tempToken.substr(0, tempToken.find(rightBracket));
                        // go to the next string after the brackets
                        line = line.substr(line.find(rightBracket) + 1);
                    }
                    if(tempToken.find(rightBracket) != -1) {
                        subTempToken = tempToken.substr(0, tempToken.find(rightBracket));
                        lexVec.push_back(subTempToken);
                        tempToken = tempToken.substr((tempToken.find(rightBracket) + 1));
                        flag = 1;
                    }
                    if(flag == 0) {
                        lexVec.push_back(tempToken);
                    }
                    line = line.substr((line.find(space) + 1));
                    // check brackets after while and remove them
                    if(strWithBrackets(line)) {
                        if((tempToken == "while") || (tempToken == "if")) {
                            flagWhile = 1;
                        }
                        token = line.substr((line.find(leftBracket) + 1), line.find(rightBracket));
                        //check inside brackets
                        if(strWithBrackets(token)) {
                            token = token.substr((token.find(leftBracket) + 1), token.find(rightBracket));
                            flag = 1;
                        }
                        token = token.substr(0, token.find(rightBracket));
                        // go to the next string after the brackets
                        line = line.substr(line.find(rightBracket) + 2);
                        // check if there is a space inside the brackets
                        if((flag == 0) && (flagWhile == 0)) {
                            while (token.find(space) != -1) {
                                tempToken = token.substr(0, token.find(space));
                                lexVec.push_back(tempToken);
                                token = token.substr((token.find(space) + 1));
                            }
                        }
                        lexVec.push_back(token);
                        flag = 0;
                    }
                    if(line == "{") {
                        lexVec.push_back(line);
                    }
                }

                // check if there is ()
                if(strWithBrackets(line)) {
                    token = line.substr(0, line.find(leftBracket));
                    // remove the space before
                    if(token.find("\t") != -1) {
                        token = token.substr((line.find("\t") + 1));
                    }
                    lexVec.push_back(token);
                    token = line.substr((line.find(leftBracket) + 1), line.find(rightBracket));
                    token = token.substr(0, token.find(rightBracket));
                    lexVec.push_back(token);
                }
                getline(file, line);
            }
            if(line == "}") {
                lexVec.push_back(line);
            }
            // go to the next line from file
            continue;
        }

        // check line with brackets and its not while or if
        if(strWithBrackets(line)) {
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
            if(token.find(space) == 0) {
                token = token.substr(token.find(space) + 1);
            }
            lexVec.push_back(token);
            token = line.substr((line.find(leftBracket) + 1), line.find(rightBracket));
            token = token.substr(0, token.find(rightBracket));
            if (strWithComma(token)) {
                token = token.substr(0, token.find(comma));
                lexVec.push_back(token);
                token = line.substr((line.find(comma) + 1));
                token = token.substr(0, token.find(rightBracket));
            }
            lexVec.push_back(token);
        }
        // check if there is =
        if(strWithEqual(line)) {
            token = line.substr(0, line.find(equal));
            // check if there is a space
            if(strWithVar(token)) {
                tempToken = token.substr(0, token.find(space));
                lexVec.push_back(tempToken);
                token = token.substr((token.find(space) + 1));
            }
            if(token.find(space) != -1) {
                token = token.substr(0,token.find(space));
            }
            lexVec.push_back(token);
            lexVec.push_back(equal);
            token = line.substr((line.find(equal) + 2));
            if(token.find(space) != -1) {
                token = token.substr(0,token.find(space));
            }
            lexVec.push_back(token);
        }
    }
    // close the file
    file.close();

    // printtttttttttttttt only for chek
//    for(int i = 0; i < lexVec.size(); i++) {
//        cout << lexVec.at(i) << endl;
//    }

    return lexVec;
}