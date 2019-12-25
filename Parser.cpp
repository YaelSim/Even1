//
// Created by yael on 19/12/2019.
//

#include "Parser.h"

map<std::__cxx11::string, struct Command *> Parser::createCommandMap() {
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
    return commandMap;
}

void Parser::parser(vector<string> lexVec) {
    map<string, Command*> commandMap = this->createCommandMap();
    int index = 0;
    string key;
    Command *curr;
    while (!lexVec.empty()) {
        key = lexVec.at(index);
        if(key != "openDataServer" && key != "connectControlClient" && key != "var" && key != "Print" &&
           key != "Sleep" && key != "if" && key != "while") {
            curr = commandMap["update"];
        } else {
            curr = commandMap[key];
        }
        if (curr != nullptr) {
            index += (*curr).execute(lexVec);
        }
        // delete from lexer what we used
        while (index != 0) {
            auto first = lexVec.begin();
            lexVec.erase(first);
            index--;
        }
    }
}

//unordered_map<string, Command*> Parser::createCommandMap(vector<string> lexVec) {
//    unordered_map<string, Command*> commandMap;
//    int spotsToJump = 1, numOfParams = 1, beginOfSub = 0;
//    for (auto i = lexVec.begin(); i != lexVec.end(); i += spotsToJump) {
//        if ((*i) == "openDataServer") {
//            numOfParams = 1;
//            string parameter = (*(i + 1));
//            Command* curr = new OpenServerCommand(parameter);
//            commandMap.insert({"openDataServer", curr});
//        } else if ((*i) == "connectControlClient") {
//            numOfParams = 2;
//            string param1 = (*(i + 1));
//            string param2 = (*(i + 2));
//            Command* curr = new ConnectCommand(param1, param2);
//            commandMap.insert({"connectControlClient", curr});
//        } else if ((*i) == "var") {
//            if ((*(i + 3)) == "sim") {
//                numOfParams = 4;
//                string param1 = (*(i + 1)); //name
//                string param2 = (*(i + 2)); //arrow
//                string param3 = (*(i + 4)); //simPath
//                Command* curr = new DefineVarCommand(param1, param2, param3, (double)NULL);
//                //Command* curr = new DefineVarCommand(param1, param2, param3, 0.0);
//                commandMap.insert({"var", curr});
//                VarObject *varObj = new VarObject(param2, param3, (double)NULL);
//                //VarObject *varObj = new VarObject(param2, param3, 0.0);
//                //Add the new var to the symbol table
//                if (param2 == "->") {
//                    this->symbolTable.actOnTxtSymbolTable(param1, *varObj);
//                } else {
//                    this->symbolTable.actOnXmlSymbolTable(param1, *varObj);
//                }
//            } else {
//                // var h0 = heading
//                numOfParams = 3;
//                VarObject* varObject;
//                string arrow, simParam;
//                double value;
//                string param1 = (*(i + 1)); //name
//                string nameOfExistingVar = (*(i +3));
//                //not exist in symbolTable
//                if (!symbolTable.findInXmlMap(nameOfExistingVar)){
//                    if (!symbolTable.findInTxtMap(nameOfExistingVar)) {
//                        exit(-1);
//                    } else {
//                        arrow = this->symbolTable.getTxtSymbolTable().at(nameOfExistingVar).getArrow();
//                        value = this->symbolTable.getTxtSymbolTable().at(nameOfExistingVar).getVarValue();
//                        simParam = this->symbolTable.getTxtSymbolTable().at(nameOfExistingVar).getSim();
//                        varObject = new VarObject(arrow, simParam, value);
//                        this->symbolTable.actOnTxtSymbolTable(param1, *varObject);
//                    }
//                } else {
//                    arrow = this->symbolTable.getXmlSymbolTable().at(nameOfExistingVar).getArrow();
//                    value = this->symbolTable.getXmlSymbolTable().at(nameOfExistingVar).getVarValue();
//                    simParam = this->symbolTable.getXmlSymbolTable().at(nameOfExistingVar).getSim();
//                    varObject = new VarObject(arrow, simParam, value);
//                    this->symbolTable.actOnXmlSymbolTable(param1, *varObject);
//                }
//                Command* curr = new DefineVarCommand(param1, arrow, simParam, value);
//                commandMap.insert({"var", curr});
//            }
//        } else if ((*i) == "Print") {
//            numOfParams = 1;
//            string param = (*(i + 1));
//            Command* curr = new PrintCommand(param);
//            commandMap.insert({"Print", curr});
//        } else if ((*i) == "Sleep") {
//            numOfParams = 1;
//            int param = convertStringToInt((*(i + 1)));
//            Command* curr = new SleepCommand(param);
//            commandMap.insert({"Sleep", curr});
//        }
//            /*else if ((*i) == "takeoff") {
//            * //BBBBBBBBBBBBBBBBBOOOOOOOOOOONNNNNNNNNNNNUUUUUUUUUUSSSSSSSSSS
//            }*/
//        else if (((*i) == "if") || ((*i) == "while")) {
//            Command* curr;
//            auto first = lexVec.cbegin() + beginOfSub;
//            auto last = lexVec.cend();
//            //sub vector
//            vector<string> subLex(first, last);
//            vector<Command*> commandsVec;
//            numOfParams = 3;
//            string param1 = (*(i + 1));
//            string condition = (*(i + 2));
//            string param2 = (*(i + 3));
//            i = i + numOfParams + 1;
//            commandsVec = this->getMatchingCommandVec(subLex);
//            i = i - numOfParams - 1;
//            if ((*i) == "if") {
//                curr = new IfCommand(param1, condition, param2, commandsVec);
//                commandMap.insert({"if", curr});
//            } else {
//                curr = new WhileCommand(param1, condition, param2, commandsVec);
//                commandMap.insert({"while", curr});
//            }
//        } else {
//            numOfParams = 2;
//            string varName = (*(i));
//            string valByStr = (*(i + 2));
//            double value = stod(valByStr);
//            //look for it at the symbol table
//            if ((!symbolTable.findInTxtMap(varName)) && (!symbolTable.findInXmlMap(varName))){
//                exit(-1);
//            }
//                //if exists - update its value
//            else {
//                string arrow = (this->symbolTable.getTxtSymbolTable()).at(varName).getArrow();
//                string simParam = (this->symbolTable.getTxtSymbolTable()).at(varName).getSim();
//                VarObject *varObj = new VarObject(arrow, simParam, value);
//                // should set with the controlClient to the simulator ----------- dont forget----------------
//                //Add it to the symbol table
//                this->symbolTable.actOnTxtSymbolTable(varName, *varObj);
//                //Add it to the commandsMap --- SHALL WE ADD IT TO THE COMMAND MAP OR JUST SET IT?????
//                //Command* curr = new DefineVarCommand(varName, arrow, simParam, value);
//                //commandMap.insert({"var", curr});
//            }
//        }
//        spotsToJump = numOfParams + 1;
//        beginOfSub += spotsToJump;
//    }
//}

//void Parser::parser(vector<string> lexVec) {
//    //unordered_map<string, Command*> commandMap = this->createCommandMap(lexVec);
//    unordered_map<string, Command*> commandMap = this->createCommandMap();
//    int index = 0;
//    while (index < lexVec.size()) {
//        Command* curr = commandMap[lexVec.at(index)];
//        //dONT lehitfalep------------------check it works properly
//        //Command* curr = commandMap[lexVec.[index]];
//        if (curr != nullptr) {
//            index += (*curr).execute();
//        }
//    }
//}


//vector<Command*> Parser::getMatchingCommandVec(vector<string> paramVector) {
//    int spotsToJump = 1, numOfParams = 1, beginOfSub = 0;
//    vector<Command*> commandVec;
//    for (auto i = paramVector.begin(); i != paramVector.end(); i += spotsToJump) {
//        if ((*i) == "openDataServer") {
//            numOfParams = 1;
//            string parameter = (*(i + 1));
//            Command* curr = new OpenServerCommand(parameter);
//            commandVec.push_back(curr);
//        } else if ((*i) == "connectControlClient") {
//            numOfParams = 2;
//            string param1 = (*(i + 1));
//            string param2 = (*(i + 2));
//            Command* curr = new ConnectCommand(param1, param2);
//            commandVec.push_back(curr);
//        } else if ((*i) == "var") {
//            if ((*(i + 3)) == "sim") {
//                numOfParams = 4;
//                string param1 = (*(i + 1)); //name
//                string param2 = (*(i + 2)); //arrow
//                string param3 = (*(i + 4)); //simPath
//                Command* curr = new DefineVarCommand(param1, param2, param3, (double)NULL);
//                //Command* curr = new DefineVarCommand(param1, param2, param3, 0.0);
//                commandVec.push_back(curr);
//                VarObject *varObj = new VarObject(param2, param3, (double)NULL);
//                //VarObject *varObj = new VarObject(param2, param3, 0.0);
//                //Add the new var to the symbol table
//                if (param2 == "->") {
//                    this->symbolTable.actOnTxtSymbolTable(param1, *varObj);
//                } else {
//                    this->symbolTable.actOnXmlSymbolTable(param1, *varObj);
//                }
//            } else {
//                // var h0 = heading
//                numOfParams = 3;
//                VarObject* varObject;
//                string arrow, simParam;
//                double value;
//                string param1 = (*(i + 1)); //name
//                string nameOfExistingVar = (*(i +3));
//                //not exist in symbolTable
//                if (!symbolTable.findInXmlMap(nameOfExistingVar)){
//                    if (!symbolTable.findInTxtMap(nameOfExistingVar)) {
//                        exit(-1);
//                    } else {
//                        arrow = this->symbolTable.getTxtSymbolTable().at(nameOfExistingVar).getArrow();
//                        value = this->symbolTable.getTxtSymbolTable().at(nameOfExistingVar).getVarValue();
//                        varObject = new VarObject(arrow, simParam, value);
//                        this->symbolTable.actOnTxtSymbolTable(param1, *varObject);
//                    }
//                } else {
//                    arrow = this->symbolTable.getXmlSymbolTable().at(nameOfExistingVar).getArrow();
//                    value = this->symbolTable.getXmlSymbolTable().at(nameOfExistingVar).getVarValue();
//                    varObject = new VarObject(arrow, simParam, value);
//                    this->symbolTable.actOnXmlSymbolTable(param1, *varObject);
//                }
//                //Add it to the commandsMap
//                Command* curr = new DefineVarCommand(param1, arrow, simParam, value);
//                commandVec.push_back(curr);
//            }
//        } else if ((*i) == "Print") {
//            numOfParams = 1;
//            string param = (*(i + 1));
//            Command* curr = new PrintCommand(param);
//            commandVec.push_back(curr);
//        } else if ((*i) == "Sleep") {
//            numOfParams = 1;
//            int param = convertStringToInt((*(i + 1)));
//            Command* curr = new SleepCommand(param);
//            commandVec.push_back(curr);
//        } else if (((*i) != "}")) {
//            return commandVec;
//        }
//            /*else if ((*i) == "takeoff") {
//            * //BBBBBBBBBBBBBBBBBOOOOOOOOOOONNNNNNNNNNNNUUUUUUUUUUSSSSSSSSSS
//            }*/
//        else {
//            numOfParams = 2;
//            string varName = (*(i));
//            string valByStr = (*(i + 2));
//            double value = stod(valByStr);
//            //look for it at the symbol table
//            //not exist in symbolTable Map
//            if ((!symbolTable.findInTxtMap(varName)) && (!symbolTable.findInXmlMap(varName))){
//                exit(-1);
//            }
//                //if exists - update its value
//            else {
//                string arrow = (this->symbolTable.getTxtSymbolTable()).at(varName).getArrow();
//                string simParam = (this->symbolTable.getTxtSymbolTable()).at(varName).getSim();
//                VarObject *varObj = new VarObject(arrow, simParam, value);
//                // should set with the controlClient to the simulator ----------- dont forget----------------
//                //Add it to the symbol table
//                this->symbolTable.actOnTxtSymbolTable(varName, *varObj);
//                //Add it to the commandsMap --- SHALL WE ADD IT TO THE COMMAND MAP OR JUST SET IT?????
//                //Command* curr = new DefineVarCommand(varName, arrow, simParam, value);
//                //commandVec.push_back(curr);
//            }
//        }
//        spotsToJump = numOfParams + 1;
//        beginOfSub += spotsToJump;
//    }
//    return commandVec;
//}
