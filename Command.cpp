//
// Created by yael on 19/12/2019.
//


#include "Command.h"
std::mutex mutex_lock;

SymbolTable symbolTable;
static bool isThreadDone = false;

int readWithServer(string portToListen) {
    //Convert the portToListen to int
    //mutex_lock.lock();
    int portAsInt;
    istringstream iss (portToListen);
    iss >> portAsInt;
    if (iss.fail()) {
        exit(1);
    }
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1) {
        return(-1);
    }
    std::cout<<"socket\n"<<std::endl;
    sockaddr_in sockAddress;
    //define as IPV4 type, define any address apprears on machine, convert (htons) port to universal type
    sockAddress.sin_family = AF_INET;
    //sockAddress.sin_addr.s_addr = INADDR_ANY;
    sockAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    sockAddress.sin_port = htons(portAsInt);
    if (bind(socketfd, (struct sockaddr *) &sockAddress, sizeof(sockAddress)) == -1) {
        return(-2);
    }
    std::cout<<"bind\n"<<std::endl;
    //making socket listen at portToListen, FOR MAXIMUM CONNECTIONS AS POSSIBLE
    if (listen(socketfd, 3) == -1) {
        return(-3);
    }
    std::cout<<"listen\n"<<std::endl;
    //accepting a client
    int clientSocket = accept(socketfd, (struct sockaddr *) &sockAddress, (socklen_t*) &sockAddress);
    if (clientSocket == -1) {
        return(-4);
    }
    std::cout<<"accept\n"<<std::endl;
    //DO SOME SHIT--- READ THE XML
    close(socketfd);
    isThreadDone = true;
    //reading from client
    char buffer[1024] = {0};
    //int valread = read( clientSocket , buffer, 1024);
    while (read( clientSocket , buffer, 1024)) {
        std::cout<<buffer<<std::endl;
        //simDataParser(buffer) - get them in a vector
        // check if buffer really contains a full line of the xml
    }
    //put the buffer in the vector

    //writing back to client
    char *hello = "Hello, I can hear you! \n";
    send(clientSocket , hello , strlen(hello) , 0 );
    std::cout<<"Hello message sent\n"<<std::endl;
    return 0;
}

//int DefineVarCommand::execute() {
//    //if brakes = 0 , we need:
//    //1. update its value in the symbol table
//    //2. check if the simParam actually had a param.
//    //3. if so, make a set call and set its value
//}


int OpenServerCommand::execute(vector<string> lexer) {
    auto i = lexer.begin();
    int numOfParams = 2;
    string port = (*(i + 1));
    std::thread openServerThread(readWithServer, port);
    openServerThread.detach();
    while (!isThreadDone) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return numOfParams;
}

int ConnectCommand::execute(vector<string> lexer) {
    auto i = lexer.begin();
    int numOfParams = 3;
    string ip = (*(i + 1));
    string port = (*(i + 2));
    return numOfParams;
}

int SleepCommand::execute(vector<string> lexer) {
    auto i = lexer.begin();
    string param = (*(i + 1));
    double paramVal;
    int numOfParams = 2, toSleep = 0;
    if (isItExpressionPattern(param)) {
        Interpreter *interpreter = new Interpreter();
        Expression *expression = nullptr;
        try {
            expression = interpreter->interpret(param);
            paramVal = expression->calculate();
            delete interpreter;
            delete expression;
            toSleep = (int)paramVal;
        } catch (const char* e) {
            if (expression != nullptr) {
                delete expression;
            }
            if (interpreter != nullptr) {
                delete interpreter;
            }
        }
    } else {
        toSleep = convertStringToInt(param);
    }
    std::this_thread::sleep_for(std::chrono::seconds(toSleep));
    return numOfParams;
}

int PrintCommand::execute(vector<string> lexer) {
    auto i = lexer.begin();
    int numOfParams = 2;
    string toPrint = (*(i + 1));
    cout << toPrint << endl;
    return numOfParams;
}

int DefineVarCommand::execute(vector<string> lexer) {
    auto i = lexer.begin();
    int numOfParams;
    if ((*(i + 3)) == "sim") {
        numOfParams = 5;
        string param1 = (*(i + 1)); //name
        string param2 = (*(i + 2)); //arrow
        string param3 = (*(i + 4)); //simPath
        VarObject *varObj = new VarObject(param2, param3, (double)NULL);
        //Add the new var to the symbol table
        if (param2 == "->") {
            symbolTable.actOnTxtSymbolTable(param1, *varObj);
        } else {
            symbolTable.actOnXmlSymbolTable(param1, *varObj);
        }
    } else {
        // var h0 = heading
        numOfParams = 4;
        VarObject* varObject;
        string arrow, simParam;
        double value;
        string param1 = (*(i + 1)); //name
        string nameOfExistingVar = (*(i +3));
        //not exist in symbolTable
        if (!symbolTable.findInXmlMap(nameOfExistingVar)){
            if (!symbolTable.findInTxtMap(nameOfExistingVar)) {
                exit(-1);
            } else {
                arrow = symbolTable.getTxtSymbolTable().at(nameOfExistingVar).getArrow();
                value = symbolTable.getTxtSymbolTable().at(nameOfExistingVar).getVarValue();
                simParam = symbolTable.getTxtSymbolTable().at(nameOfExistingVar).getSim();
                varObject = new VarObject(arrow, simParam, value);
                symbolTable.actOnTxtSymbolTable(param1, *varObject);
            }
        } else {
            arrow = symbolTable.getXmlSymbolTable().at(nameOfExistingVar).getArrow();
            value = symbolTable.getXmlSymbolTable().at(nameOfExistingVar).getVarValue();
            simParam = symbolTable.getXmlSymbolTable().at(nameOfExistingVar).getSim();
            varObject = new VarObject(arrow, simParam, value);
            symbolTable.actOnXmlSymbolTable(param1, *varObject);
        }
    }
    return numOfParams;
}

int IfCommand::execute(vector<string> lexer) {
    auto i = lexer.begin();
    int numOfParams = 6;
    int index = 5;
    string param1 = (*(i + 1));
    string condition = (*(i + 2));
    string param2 = (*(i + 3));
    double param1Val, param2Val;
    param1Val = this->getParametersValue(param1);
    param2Val = this->getParametersValue(param2);
    if (isConditionTrue(param1Val, param2Val, condition)) {
        while (index != 0) {
            auto first = lexer.begin();
            lexer.erase(first);
            index--;
        }
        map<string, Command*> commandMap = this->commandsForLoops(lexer);
        string key = lexer.at(index);
        if(key == "}") {
            return numOfParams;
        }
        Command *curr = commandMap[key];
        if (curr != nullptr) {
            index += (*curr).execute(lexer);
            numOfParams += index;
        }
    }
    return numOfParams;
}

int WhileCommand::execute(vector<string> lexer) {
    auto i = lexer.begin();
    int numOfParams = 6;
    int index = 5;
    int flag = 0;
    string param1 = (*(i + 1));
    string condition = (*(i + 2));
    string param2 = (*(i + 3));
    double param1Val, param2Val;
    param1Val = this->getParametersValue(param1);
    param2Val = this->getParametersValue(param2);
    while (isConditionTrue(param1Val, param2Val, condition)) {
        while (index != 0 && flag == 0) {
            auto first = lexer.begin();
            lexer.erase(first);
            index--;
            flag = 1;
        }
        map<string, Command*> commandMap = this->commandsForLoops(lexer);
        string key = lexer.at(index);
        if(key == "}") {
            continue;
        }
        Command *curr = commandMap[key];
        if (curr != nullptr) {
            index += (*curr).execute(lexer);
            numOfParams += index;
        }
        param1Val = this->getParametersValue(param1);
        param2Val = this->getParametersValue(param2);
    }
    return numOfParams;
}

int UpdateVarCommand::execute(vector<string> lexer) {
    // set the value in the symbleTableMap
    int numOfParams = 3;

    return numOfParams;
}

int Command::convertStringToInt(string toConvert) {
    int asInt;
    istringstream iss (toConvert);
    iss >> asInt;
    if (iss.fail()) {
        exit(1);
    }
    return asInt;
}

bool Command::isItVarNamePattern(string potential) {
    regex varNamePattern("^(?![0-9])([a-zA-Z]|_|[0-9])+");
    return regex_match(potential, varNamePattern);
}

double ConditionParser::getParametersValue(string param) {
    double paramVal;
    if (isItVarNamePattern(param)) {
        if (symbolTable.findInTxtMap(param)) {
            paramVal = symbolTable.getTxtSymbolTable().at(param).getVarValue();
        } else if (symbolTable.findInXmlMap(param)) {
            paramVal = symbolTable.getXmlSymbolTable().at(param).getVarValue();
        }
    } else if (isItExpressionPattern(param)) {
        Interpreter *interpreter = new Interpreter();
        Expression *expression = nullptr;
        try {
            expression = interpreter->interpret(param);
            paramVal = expression->calculate();
            delete interpreter;
            delete expression;
        } catch (const char* e) {
            if (expression != nullptr) {
                delete expression;
            }
            if (interpreter != nullptr) {
                delete interpreter;
            }
        }
    } else {
        paramVal = stod(param);
    }
    return paramVal;
}

map<string, Command *> ConditionParser::commandsForLoops(vector<string> lexer) {
    map<string, Command *> commandMap;
    Command* curr;
    curr = new OpenServerCommand();
    commandMap.insert({"openDataServer", curr});
    curr = new ConnectCommand();
    commandMap.insert({"connectControlClient", curr});
    curr = new PrintCommand();
    commandMap.insert({"Print", curr});
    curr = new SleepCommand();
    commandMap.insert({"Sleep", curr});
    curr = new UpdateVarCommand();
    commandMap.insert({"update", curr});
    return commandMap;
}

bool ConditionParser::isConditionTrue(double param1Val, double param2Val, string condition) {
    bool isConditionTrue = false;
    if(condition == "!=") {
        if (param1Val != param2Val) {
            isConditionTrue = true;
        }
    }
    else if(condition == "==") {
        if (param1Val == param2Val) {
            isConditionTrue = true;
        }
    }
    else if(condition == "<=") {
        if (param1Val <= param2Val) {
            isConditionTrue = true;
        }
    }
    else if(condition == ">=") {
        if (param1Val >= param2Val) {
            isConditionTrue = true;
        }
    }
    else if(condition == "<") {
        if (param1Val < param2Val) {
            isConditionTrue = true;
        }
    }
    else if(condition == ">") {
        if (param1Val > param2Val) {
            isConditionTrue = true;
        }
    }
    return isConditionTrue;
}

bool Command::isItExpressionPattern(string potential) {
    char plusOp = '+', minusOp = '-', multOp = '*', divOp = '/';
    if (potential.find(plusOp) != std::string::npos) {
        return true;
    }
    if (potential.find(minusOp) != std::string::npos) {
        return true;
    }
    if (potential.find(multOp) != std::string::npos) {
        return true;
    }
    return potential.find(divOp) != std::string::npos;
}
