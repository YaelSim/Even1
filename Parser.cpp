//
// Created by yael and linoy on 19/12/2019.
//

#include "Parser.h"

// in this method we create our command map
map<string,Command*> Parser::createCommandMap() {
    map<string, Command*> commandMap;
    Command* curr;
    curr = new OpenServerCommand();
    commandMap.insert({"openDataServer", curr});
    curr = new ConnectCommand();
    commandMap.insert({"connectControlClient", curr});
    curr = new DefineVarCommand();
    commandMap.insert({"var", curr});
    curr = new PrintCommand();
    commandMap.insert({"Print", curr});
    curr = new SleepCommand();
    commandMap.insert({"Sleep", curr});
    curr = new IfCommand();
    commandMap.insert({"if", curr});
    curr = new WhileCommand();
    commandMap.insert({"while", curr});
    curr = new UpdateVarCommand();
    commandMap.insert({"update", curr});
    // return the map
    return commandMap;
}

//in this method we execute the commands by the map and lexer
void Parser::parser(vector<string> lexVec) {
    createBufferXmlMap();
    map<string, Command*> commandMap = this->createCommandMap();
    int index = 0;
    string key;
    Command *curr;
    while (!lexVec.empty()) {
        key = lexVec.at(index);
        if(key != "openDataServer" && key != "connectControlClient" && key != "var" && key != "Print" &&
           key != "Sleep" && key != "if" && key != "while") {
            // in case we just update var like in case of breaks = 0
            curr = commandMap["update"];
        } else {
            curr = commandMap[key];
        }
        if (curr != nullptr) {
            // go to the command's execute
            index += (*curr).execute(lexVec);
        }
        // delete from lexer what already we used
        while (index != 0) {
            auto first = lexVec.begin();
            lexVec.erase(first);
            index--;
        }
    }
    // the program has finished (printed done)
    isProgFinished = true;
}