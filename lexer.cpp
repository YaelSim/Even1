//
// Created by yael and linoy on 19/12/2019.
//
#include "lexer.h"
#include <iostream>
#include <string>
#include <regex>
#include <vector>

using namespace std;

// regex to check if there is brackets in the string
bool Lexer::strWithBrackets(string str) {
    regex check("(.*)\\((.*)\\)(.*)");
    return regex_match(str, check);
}
//regex to check if there is comma in the string
bool Lexer::strWithComma(string str) {
    regex check("(.*)(\\,)(.*)");
    return regex_match(str, check);
}
//regex to check if there is left arrow in the string
bool Lexer::strWithLeftArrow(string str) {
    regex check("(.*)(<-)(.*)");
    return regex_match(str, check);
}
//regex to check if there is right arrow in the string
bool Lexer::strWithRightArrow(string str) {
    regex check("(.*)(->)(.*)");
    return regex_match(str, check);
}
//regex to check if there is equal sign in the string
bool Lexer::strWithEqual(string str) {
    regex check("(.*)[^<>](=)(.*)");
    return regex_match(str, check);
}
//regex to check if there is double equals in the string
bool Lexer::strWithDoubleEqual(string str) {
    regex check("(.*)[^<>](==)(.*)");
    return regex_match(str, check);
}
//regex to check if there is "while" in the string
bool Lexer::strWithWhile(string str) {
    regex check("(.*)(while)(.*)");
    return regex_match(str, check);
}
//regex to check if there is "var" in the string
bool Lexer::strWithVar(string str) {
    regex check("(.*)(var)(.*)");
    return regex_match(str, check);
}
//regex to check if there is "if" in the string
bool Lexer::strWithIf(string str) {
    regex check("(.*)(if)(.*)");
    return regex_match(str, check);
}
// this method takes the text and convert it to a vector of strings
vector<string> Lexer::lexer(string fileName) {
    string line, token, tempToken, subTempToken, leftBracket = "(", rightBracket = ")", comma = ",", quote = "\"";
    string leftArrow = "<-", rightArrow = "->", space = " ", equal = "=", doubleEqual = "==";
    int flag, flagWhile, printFlag, pushFlag;
    vector<string> lexVec;
    ifstream file;
    // ofen the text file
    file.open(fileName, ios::in);
    if (!file) {
        cout << "can't open the file - no arguments" << endl;
        exit(1);
    }

    // now we will read the file and push into lexer
    while (!file.eof()) {
        flag = 0;
        flagWhile = 0;
        printFlag = 0;
        pushFlag = 0;
        getline(file, line);
        if(strWithWhile(line) || strWithIf(line)) {
            // do until the end of the while
            while(line != "}") {
                // take off all the spaces in the beginning
                while(line.find(space) == 0) {
                    line = line.substr(line.find(space) + 1);
                }
                // check if there is =
                if(strWithEqual(line)) {
                    token = line.substr(0, line.find(equal));
                    // remove the tabs before
                    if(token.find('\t') != -1) {
                        token = token.substr((line.find('\t') + 1));
                    }
                    // remove the spaces before
                    if(token.find(space) != -1) {
                        subTempToken = token.substr(token.find(space) + 1);
                        if(subTempToken.find(space) != -1) {
                            subTempToken = subTempToken.substr(0, subTempToken.find(space));
                        }
                        token = token.substr(0,token.find(space));
                    }
                    lexVec.push_back(token);
                    if(subTempToken != "" && subTempToken != token) {
                        lexVec.push_back(subTempToken);
                    }
                    //push double == if exist
                    if(strWithDoubleEqual(line)) {
                        lexVec.push_back(doubleEqual);
                        line = line.substr((line.find(equal) + 1));
                    } else {
                        lexVec.push_back(equal);
                    }
                    line = line.substr((line.find(equal) + 1));
                    if(line.find('{') != -1) {
                        line = line.substr(0,line.find('{'));
                        pushFlag = 1;
                    }
                    if(line.find(space) == 0) {
                        line = line.substr(line.find(space) + 1);
                    }
                    lexVec.push_back(line);
                    // we push it  beacuse remove before
                    if(pushFlag == 1) {
                        lexVec.push_back("{");
                        pushFlag = 0;
                    }
                    // go to the next line
                    getline(file, line);
                    continue;
                }

                while (line.find(space) != -1) {
                    //take off a lot of space in the beginning
                    while(line.find(space) == 0) {
                        line = line.substr(line.find(space) + 1);
                    }
                    tempToken = line.substr(0, line.find(space));
                    // take off the brackets
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

                // check if there is brackets
                if(strWithBrackets(line)) {
                    token = line.substr(0, line.find(leftBracket));
                    // remove the tabs before
                    if(token.find('\t') != -1) {
                        token = token.substr((line.find('\t') + 1));
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

        // check if there is equal sign
        if(strWithEqual(line) && !strWithVar(line)) {
            token = line.substr(0, line.find(equal));
            // remove the tabs before
            if(token.find('\t') != -1) {
                token = token.substr((line.find('\t') + 1));
            }
            // remove the space before
            while (token.find(space) == 0 ) {
                token = token.substr(token.find(space) + 1);
            }
            if(token.find(space) != -1) {
                token = token.substr(0,token.find(space));
            }
            lexVec.push_back(token);
            lexVec.push_back(equal);
            line = line.substr((line.find(equal) + 2));
            std::string::iterator end_pos = std::remove(line.begin(), line.end(), ' ');
            line.erase(end_pos, line.end());
            lexVec.push_back(line);
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
                    if(tempToken.find(space) != -1) {
                        tempToken = tempToken.substr(0,tempToken.find(space));
                    }
                    lexVec.push_back(tempToken);
                }
                token = token.substr((token.find(leftArrow) + 2));
                lexVec.push_back(leftArrow);
            }
            // check if there is arrow in the string and split with it
            if(strWithRightArrow(token)) {
                tempToken = token.substr(0, token.find(rightArrow));
                // check if there is a space
                if(tempToken.find(space) != -1) {
                    subTempToken = tempToken.substr(0, tempToken.find(space));
                    lexVec.push_back(subTempToken);
                    tempToken = tempToken.substr((tempToken.find(space) + 1));
                    if(tempToken.find(space) != -1) {
                        tempToken = tempToken.substr(0,tempToken.find(space));
                    }
                    lexVec.push_back(tempToken);
                }
                token = token.substr((token.find(rightArrow) + 2));
                lexVec.push_back(rightArrow);
            }
            // take off the spaces before
            if(token.find(space) == 0) {
                token = token.substr(token.find(space) + 1);
            }
            lexVec.push_back(token);
            token = line.substr((line.find(leftBracket) + 1), line.find(rightBracket));
            token = token.substr(0, token.find(rightBracket));
            // if its print or sleep no need to split with comma
            if(lexVec.back() == "print" || lexVec.back() == "Print" ||
                lexVec.back() == "sleep" || lexVec.back() == "Sleep") {
                printFlag = 1;
            }
            if (strWithComma(token) && printFlag == 0) {
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
            // take off the spaces
            if(token.find(space) != -1) {
                token = token.substr(0,token.find(space));
            }
            lexVec.push_back(token);
            lexVec.push_back(equal);
            token = line.substr((line.find(equal) + 2));
            // take off the spaces
            if(token.find(space) != -1) {
                token = token.substr(0,token.find(space));
            }
            lexVec.push_back(token);
        }
    }
    // close the file and return the vector we got
    file.close();
    return lexVec;
}
