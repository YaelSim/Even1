//
// Created by yael and linoy on 19/12/2019.
//

#include "Command.h"
//Global variables
SymbolTable symbolTable;
bool isThreadDone = false;
bool isProgFinished = false;
list<string> sendToSim;
std::mutex progMutex;

//This static method enables receiving details from the simulator.
int readWithServer(string portToListen) {
    int portAsInt, index = 0, option = 1;
    istringstream iss (portToListen);
    //Converting the portToListen to int type.
    iss >> portAsInt;
    if (iss.fail()) {
        exit(1);
    }
    //Creation of socket
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1) {
        exit(-1);
    }
    std::cout<<"socket\n"<<std::endl;

    // Attaching socket
    if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option))) {
        exit(-8);
    }
    std::cout<<"socket setsockopt\n"<<std::endl;

    struct sockaddr_in sockAddress;
    sockAddress.sin_family = AF_INET;
    //sockAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    sockAddress.sin_addr.s_addr = INADDR_ANY;
    sockAddress.sin_port = htons(portAsInt);
    if (bind(socketfd, (struct sockaddr *) &sockAddress, sizeof(sockAddress)) < 0) {
        exit(-2);
    }
    std::cout<<"bind\n"<<std::endl;

    //Listening for clients!
    if (listen(socketfd, 3) < 0) {
        exit(-3);
    }
    std::cout<<"listen\n"<<std::endl;

    //Accepting a client
    int clientSocket = accept(socketfd, (struct sockaddr *) &sockAddress, (socklen_t*) &sockAddress);
    if (clientSocket < 0) {
        exit(-4);
    }
    std::cout<<"accept\n"<<std::endl;
    close(socketfd);
    isThreadDone = true;
    //reading from client as long as the parser function didn't end its' work
    while (!isProgFinished) {
        index = 0;
        char buffer[1] = {0};
        char totalBuffer[1024] = {0};
        int val_read = read(clientSocket, buffer, 1);
        while (buffer[0] != '\n') {
            totalBuffer[index] = buffer[0];
            index++;
            val_read = read(clientSocket, buffer, 1);
        }
        vector<string> splitBufferVec = splitBuffer(totalBuffer, ',');
        // update the map by the values we get from buffer
        updateValsOfBufferXmlMap(splitBufferVec);
    }
    return 0;
}

//This static method receives an array and divides its content into a new Vector<string> (and returns it accordingly).
vector<string> splitBuffer(char buffer[1024], char comma) {
    vector<string> vec;
    size_t start;
    //Convert char[] buffer to string bufferAsString
    string bufferAsString;
    stringstream stringstream;
    stringstream << buffer;
    stringstream >> bufferAsString;
    size_t end = 0;
    //push the value that was found to the vector
    while ((start = bufferAsString.find_first_not_of(comma, end)) != std::string::npos) {
        end = bufferAsString.find(comma, start);
        vec.push_back(bufferAsString.substr(start, end - start));
    }
    return vec;
}

//This static method "fills" the SymbolTable's bufferXmlMap with keys according to generic_small.xml
void createBufferXmlMap() {
    string arrow = "<-";
    double zeroVal = 0.0;
    //Fill up the xml map of the symbolTable.
    auto varObject = new VarObject(arrow, "/instrumentation/airspeed-indicator/indicated-speed-kt", zeroVal);
    symbolTable.actOnXmlSymbolTable("/instrumentation/airspeed-indicator/indicated-speed-kt", varObject);

    varObject = new VarObject(arrow, "/instrumentation/altimeter/indicated-altitude-ft", zeroVal);
    symbolTable.actOnXmlSymbolTable("/instrumentation/altimeter/indicated-altitude-ft", varObject);

    varObject = new VarObject(arrow, "/sim/time/warp", zeroVal);
    symbolTable.actOnXmlSymbolTable("/sim/time/warp", varObject);

    varObject = new VarObject(arrow, "/controls/switches/magnetos", zeroVal);
    symbolTable.actOnXmlSymbolTable("/controls/switches/magnetos", varObject);

    varObject = new VarObject(arrow, "/instrumentation/heading-indicator/offset-deg", zeroVal);
    symbolTable.actOnXmlSymbolTable("/instrumentation/heading-indicator/offset-deg", varObject);

    varObject = new VarObject(arrow, "/instrumentation/altimeter/pressure-alt-ft", zeroVal);
    symbolTable.actOnXmlSymbolTable("/instrumentation/altimeter/pressure-alt-ft", varObject);

    varObject = new VarObject(arrow, "/instrumentation/attitude-indicator/indicated-pitch-deg", zeroVal);
    symbolTable.actOnXmlSymbolTable("/instrumentation/attitude-indicator/indicated-pitch-deg", varObject);

    varObject = new VarObject(arrow, "/instrumentation/attitude-indicator/indicated-roll-deg", zeroVal);
    symbolTable.actOnXmlSymbolTable("/instrumentation/attitude-indicator/indicated-roll-deg", varObject);

    varObject = new VarObject(arrow, "/instrumentation/attitude-indicator/internal-pitch-deg", zeroVal);
    symbolTable.actOnXmlSymbolTable("/instrumentation/attitude-indicator/internal-pitch-deg", varObject);

    varObject = new VarObject(arrow, "/instrumentation/attitude-indicator/internal-roll-deg", zeroVal);
    symbolTable.actOnXmlSymbolTable("/instrumentation/attitude-indicator/internal-roll-deg", varObject);

    varObject = new VarObject(arrow, "/instrumentation/encoder/indicated-altitude-ft", zeroVal);
    symbolTable.actOnXmlSymbolTable("/instrumentation/encoder/indicated-altitude-ft", varObject);

    varObject = new VarObject(arrow, "/instrumentation/encoder/pressure-alt-ft", zeroVal);
    symbolTable.actOnXmlSymbolTable("/instrumentation/encoder/pressure-alt-ft", varObject);

    varObject = new VarObject(arrow, "/instrumentation/gps/indicated-altitude-ft", zeroVal);
    symbolTable.actOnXmlSymbolTable("/instrumentation/gps/indicated-altitude-ft", varObject);

    varObject = new VarObject(arrow, "/instrumentation/gps/indicated-ground-speed-kt", zeroVal);
    symbolTable.actOnXmlSymbolTable("/instrumentation/gps/indicated-ground-speed-kt", varObject);

    varObject = new VarObject(arrow, "/instrumentation/gps/indicated-vertical-speed", zeroVal);
    symbolTable.actOnXmlSymbolTable("/instrumentation/gps/indicated-vertical-speed", varObject);

    varObject = new VarObject(arrow, "/instrumentation/heading-indicator/indicated-heading-deg", zeroVal);
    symbolTable.actOnXmlSymbolTable("/instrumentation/heading-indicator/indicated-heading-deg", varObject);

    varObject = new VarObject(arrow, "/instrumentation/magnetic-compass/indicated-heading-deg", zeroVal);
    symbolTable.actOnXmlSymbolTable("/instrumentation/magnetic-compass/indicated-heading-deg", varObject);

    varObject = new VarObject(arrow, "/instrumentation/slip-skid-ball/indicated-slip-skid", zeroVal);
    symbolTable.actOnXmlSymbolTable("/instrumentation/slip-skid-ball/indicated-slip-skid", varObject);

    varObject = new VarObject(arrow, "/instrumentation/turn-indicator/indicated-turn-rate", zeroVal);
    symbolTable.actOnXmlSymbolTable("/instrumentation/turn-indicator/indicated-turn-rate", varObject);

    varObject = new VarObject(arrow, "/instrumentation/vertical-speed-indicator/indicated-speed-fpm", zeroVal);
    symbolTable.actOnXmlSymbolTable("/instrumentation/vertical-speed-indicator/indicated-speed-fpm", varObject);

    varObject = new VarObject(arrow, "/controls/flight/aileron", zeroVal);
    symbolTable.actOnXmlSymbolTable("/controls/flight/aileron", varObject);

    varObject = new VarObject(arrow, "/controls/flight/elevator", zeroVal);
    symbolTable.actOnXmlSymbolTable("/controls/flight/elevator", varObject);

    varObject = new VarObject(arrow, "/controls/flight/rudder", zeroVal);
    symbolTable.actOnXmlSymbolTable("/controls/flight/rudder", varObject);

    varObject = new VarObject(arrow, "/controls/flight/flaps", zeroVal);
    symbolTable.actOnXmlSymbolTable("/controls/flight/flaps", varObject);

    varObject = new VarObject(arrow, "/controls/engines/engine/c", zeroVal);
    symbolTable.actOnXmlSymbolTable("/controls/engines/engine/throttle", varObject);

    varObject = new VarObject(arrow, "/controls/engines/current-engine/throttle", zeroVal);
    symbolTable.actOnXmlSymbolTable("/controls/engines/current-engine/throttle", varObject);

    varObject = new VarObject(arrow, "/controls/switches/master-avionics", zeroVal);
    symbolTable.actOnXmlSymbolTable("/controls/switches/master-avionics", varObject);

    varObject = new VarObject(arrow, "/controls/switches/starter", zeroVal);
    symbolTable.actOnXmlSymbolTable("/controls/switches/starter", varObject);

    varObject = new VarObject(arrow, "/engines/active-engine/auto-start", zeroVal);
    symbolTable.actOnXmlSymbolTable("/engines/active-engine/auto-start", varObject);

    varObject = new VarObject(arrow, "/controls/flight/speedbrake", zeroVal);
    symbolTable.actOnXmlSymbolTable("/controls/flight/speedbrake", varObject);

    varObject = new VarObject(arrow, "/sim/model/c172p/brake-parking", zeroVal);
    symbolTable.actOnXmlSymbolTable("/sim/model/c172p/brake-parking", varObject);

    varObject = new VarObject(arrow, "/controls/engines/engine/primer", zeroVal);
    symbolTable.actOnXmlSymbolTable("/controls/engines/engine/primer", varObject);

    varObject = new VarObject(arrow, "/controls/engines/current-engine/mixture", zeroVal);
    symbolTable.actOnXmlSymbolTable("/controls/engines/current-engine/mixture", varObject);

    varObject = new VarObject(arrow, "/controls/switches/master-bat", zeroVal);
    symbolTable.actOnXmlSymbolTable("/controls/switches/master-bat", varObject);

    varObject = new VarObject(arrow, "/controls/switches/master-alt", zeroVal);
    symbolTable.actOnXmlSymbolTable("/controls/switches/master-alt", varObject);

    varObject = new VarObject(arrow, "/engines/engine/rpm", zeroVal);
    symbolTable.actOnXmlSymbolTable("/engines/engine/rpm", varObject);
}

//This static method receives a vector<string> and updates the SymbolTable's bufferXmlMap according to the given vector.
void updateValsOfBufferXmlMap(vector<string> vec) {
    VarObject* varObject = symbolTable.getXmlSymbolTable()["/instrumentation/airspeed-indicator/indicated-speed-kt"];
    varObject->setVarValue(stof(vec.at(0)));
    symbolTable.getXmlSymbolTable().at("/instrumentation/airspeed-indicator/indicated-speed-kt") = varObject;

    varObject = symbolTable.getXmlSymbolTable()["/sim/time/warp"];
    varObject->setVarValue(stof(vec.at(1)));
    symbolTable.getXmlSymbolTable().at("/sim/time/warp") = varObject;

    varObject = symbolTable.getXmlSymbolTable()["/controls/switches/magnetos"];
    varObject->setVarValue(stof(vec.at(2)));
    symbolTable.getXmlSymbolTable().at("/controls/switches/magnetos") = varObject;

    varObject = symbolTable.getXmlSymbolTable()["/instrumentation/heading-indicator/offset-deg"];
    varObject->setVarValue(stof(vec.at(3)));
    symbolTable.getXmlSymbolTable().at("/instrumentation/heading-indicator/offset-deg") = varObject;

    varObject = symbolTable.getXmlSymbolTable()["/instrumentation/altimeter/indicated-altitude-ft"];
    varObject->setVarValue(stof(vec.at(4)));
    symbolTable.getXmlSymbolTable().at("/instrumentation/altimeter/indicated-altitude-ft") = varObject;

    varObject = symbolTable.getXmlSymbolTable()["/instrumentation/altimeter/pressure-alt-ft"];
    varObject->setVarValue(stof(vec.at(5)));
    symbolTable.getXmlSymbolTable().at("/instrumentation/altimeter/pressure-alt-ft") = varObject;

    varObject = symbolTable.getXmlSymbolTable()["/instrumentation/attitude-indicator/indicated-pitch-deg"];
    varObject->setVarValue(stof(vec.at(6)));
    symbolTable.getXmlSymbolTable().at("/instrumentation/attitude-indicator/indicated-pitch-deg") = varObject;

    varObject = symbolTable.getXmlSymbolTable()["/instrumentation/attitude-indicator/indicated-roll-deg"];
    varObject->setVarValue(stof(vec.at(7)));
    symbolTable.getXmlSymbolTable().at("/instrumentation/attitude-indicator/indicated-roll-deg") = varObject;

    varObject = symbolTable.getXmlSymbolTable()["/instrumentation/attitude-indicator/internal-pitch-deg"];
    varObject->setVarValue(stof(vec.at(8)));
    symbolTable.getXmlSymbolTable().at("/instrumentation/attitude-indicator/internal-pitch-deg") = varObject;

    varObject = symbolTable.getXmlSymbolTable()["/instrumentation/attitude-indicator/internal-roll-deg"];
    varObject->setVarValue(stof(vec.at(9)));
    symbolTable.getXmlSymbolTable().at("/instrumentation/attitude-indicator/internal-roll-deg") = varObject;

    varObject = symbolTable.getXmlSymbolTable()["/instrumentation/encoder/indicated-altitude-ft"];
    varObject->setVarValue(stof(vec.at(10)));
    symbolTable.getXmlSymbolTable().at("/instrumentation/encoder/indicated-altitude-ft") = varObject;

    varObject = symbolTable.getXmlSymbolTable()["/instrumentation/encoder/pressure-alt-ft"];
    varObject->setVarValue(stof(vec.at(11)));
    symbolTable.getXmlSymbolTable().at("/instrumentation/encoder/pressure-alt-ft") = varObject;

    varObject = symbolTable.getXmlSymbolTable()["/instrumentation/gps/indicated-altitude-ft"];
    varObject->setVarValue(stof(vec.at(12)));
    symbolTable.getXmlSymbolTable().at("/instrumentation/gps/indicated-altitude-ft") = varObject;

    varObject = symbolTable.getXmlSymbolTable()["/instrumentation/gps/indicated-ground-speed-kt"];
    varObject->setVarValue(stof(vec.at(13)));
    symbolTable.getXmlSymbolTable().at("/instrumentation/gps/indicated-ground-speed-kt") = varObject;

    varObject = symbolTable.getXmlSymbolTable()["/instrumentation/gps/indicated-vertical-speed"];
    varObject->setVarValue(stof(vec.at(14)));
    symbolTable.getXmlSymbolTable().at("/instrumentation/gps/indicated-vertical-speed") = varObject;

    varObject = symbolTable.getXmlSymbolTable()["/instrumentation/heading-indicator/indicated-heading-deg"];
    varObject->setVarValue(stof(vec.at(15)));
    symbolTable.getXmlSymbolTable().at("/instrumentation/heading-indicator/indicated-heading-deg") = varObject;

    varObject = symbolTable.getXmlSymbolTable()["/instrumentation/magnetic-compass/indicated-heading-deg"];
    varObject->setVarValue(stof(vec.at(16)));
    symbolTable.getXmlSymbolTable().at("/instrumentation/magnetic-compass/indicated-heading-deg") = varObject;

    varObject = symbolTable.getXmlSymbolTable()["/instrumentation/slip-skid-ball/indicated-slip-skid"];
    varObject->setVarValue(stof(vec.at(17)));
    symbolTable.getXmlSymbolTable().at("/instrumentation/slip-skid-ball/indicated-slip-skid") = varObject;

    varObject = symbolTable.getXmlSymbolTable()["/instrumentation/turn-indicator/indicated-turn-rate"];
    varObject->setVarValue(stof(vec.at(18)));
    symbolTable.getXmlSymbolTable().at("/instrumentation/turn-indicator/indicated-turn-rate") = varObject;

    varObject = symbolTable.getXmlSymbolTable()["/instrumentation/vertical-speed-indicator/indicated-speed-fpm"];
    varObject->setVarValue(stof(vec.at(19)));
    symbolTable.getXmlSymbolTable().at("/instrumentation/vertical-speed-indicator/indicated-speed-fpm") = varObject;

    varObject = symbolTable.getXmlSymbolTable()["/controls/flight/aileron"];
    varObject->setVarValue(stof(vec.at(20)));
    symbolTable.getXmlSymbolTable().at("/controls/flight/aileron") = varObject;

    varObject = symbolTable.getXmlSymbolTable()["/controls/flight/elevator"];
    varObject->setVarValue(stof(vec.at(21)));
    symbolTable.getXmlSymbolTable().at("/controls/flight/elevator") = varObject;

    varObject = symbolTable.getXmlSymbolTable()["/controls/flight/rudder"];
    varObject->setVarValue(stof(vec.at(22)));
    symbolTable.getXmlSymbolTable().at("/controls/flight/rudder") = varObject;

    varObject = symbolTable.getXmlSymbolTable()["/controls/flight/flaps"];
    varObject->setVarValue(stof(vec.at(23)));
    symbolTable.getXmlSymbolTable().at("/controls/flight/flaps") = varObject;

    varObject = symbolTable.getXmlSymbolTable()["/controls/engines/engine/throttle"];
    varObject->setVarValue(stof(vec.at(24)));
    symbolTable.getXmlSymbolTable().at("/controls/engines/engine/throttle") = varObject;

    varObject = symbolTable.getXmlSymbolTable()["/controls/engines/current-engine/throttle"];
    varObject->setVarValue(stof(vec.at(25)));
    symbolTable.getXmlSymbolTable().at("/controls/engines/current-engine/throttle") = varObject;

    varObject = symbolTable.getXmlSymbolTable()["/controls/switches/master-avionics"];
    varObject->setVarValue(stof(vec.at(26)));
    symbolTable.getXmlSymbolTable().at("/controls/switches/master-avionics") = varObject;

    varObject = symbolTable.getXmlSymbolTable()["/controls/switches/starter"];
    varObject->setVarValue(stof(vec.at(27)));
    symbolTable.getXmlSymbolTable().at("/controls/switches/starter") = varObject;

    varObject = symbolTable.getXmlSymbolTable()["/engines/active-engine/auto-start"];
    varObject->setVarValue(stof(vec.at(28)));
    symbolTable.getXmlSymbolTable().at("/engines/active-engine/auto-start") = varObject;

    varObject = symbolTable.getXmlSymbolTable()["/controls/flight/speedbrake"];
    varObject->setVarValue(stof(vec.at(29)));
    symbolTable.getXmlSymbolTable().at("/controls/flight/speedbrake") = varObject;

    varObject = symbolTable.getXmlSymbolTable()["/sim/model/c172p/brake-parking"];
    varObject->setVarValue(stof(vec.at(30)));
    symbolTable.getXmlSymbolTable().at("/sim/model/c172p/brake-parking") = varObject;

    varObject = symbolTable.getXmlSymbolTable()["/controls/engines/engine/primer"];
    varObject->setVarValue(stof(vec.at(31)));
    symbolTable.getXmlSymbolTable().at("/controls/engines/engine/primer") = varObject;

    varObject = symbolTable.getXmlSymbolTable()["/controls/engines/current-engine/mixture"];
    varObject->setVarValue(stof(vec.at(32)));
    symbolTable.getXmlSymbolTable().at("/controls/engines/current-engine/mixture") = varObject;

    varObject = symbolTable.getXmlSymbolTable()["/controls/switches/master-bat"];
    varObject->setVarValue(stof(vec.at(33)));
    symbolTable.getXmlSymbolTable().at("/controls/switches/master-bat") = varObject;

    varObject = symbolTable.getXmlSymbolTable()["/controls/switches/master-alt"];
    varObject->setVarValue(stof(vec.at(34)));
    symbolTable.getXmlSymbolTable().at("/controls/switches/master-alt") = varObject;

    varObject = symbolTable.getXmlSymbolTable()["/engines/engine/rpm"];
    varObject->setVarValue(stof(vec.at(35)));
    symbolTable.getXmlSymbolTable().at("/engines/engine/rpm") = varObject;
}

//This static method enables a connection between a socket and the simulator,
// in order to set variables' values during the program.
int readWithClient(string ip, string portToListen) {
    int portAsInt;
    istringstream iss (portToListen);
    //Convert the given portToListen to int type
    iss >> portAsInt;
    if (iss.fail()) {
        exit(1);
    }
    //Remove "..."
    std::string::iterator end_pos = std::remove(ip.begin(), ip.end(), '"');
    ip.erase(end_pos, ip.end());
    const char* ipAddress = ip.c_str();
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == 0) {
        exit (-1);
    }
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ipAddress);
    address.sin_port = htons(portAsInt);
    int is_connect = connect(client_socket, (struct sockaddr *)&address, sizeof(address));
    if (is_connect == -1) {
        exit (-2);
    } else {
        std::cout<<"Client is now connected to server" <<std::endl;
    }
    //As long as the parser function didn't end up its' job
    while (!isProgFinished) {
        //As long as the list of messages is not empty, send each node to the simulator
        if (!sendToSim.empty()) {
            progMutex.lock();
            string currStr = sendToSim.front();
            currStr += "\r\n";
            const char* currAsChars = currStr.c_str();
            sendToSim.pop_front();
            int is_sent = send(client_socket , currAsChars , strlen(currAsChars) , 0);
            progMutex.unlock();
            if (is_sent == -1) {
               std::cout<<"Error sending message"<<std::endl;
            }
        }
    }
    //close the socket when finished the program
    if(isProgFinished) {
        close(client_socket);
    }
    return 0;
}

//This method provides an implementation of executing OpenServerCommand.
int OpenServerCommand::execute(vector<string> lexer) {
    auto i = lexer.begin();
    int numOfParams = 2;
    double paramVal;
    string port = (*(i + 1));
    //Is the port given as an expression (38+324+3434)..
    if (isItExpressionPattern(port)) {
        Interpreter *interpreter = new Interpreter();
        Expression *expression = nullptr;
        try {
            //remove spaces and return the expression after calculation
            std::string::iterator end_pos = std::remove(port.begin(), port.end(), ' ');
            port.erase(end_pos, port.end());
            expression = interpreter->interpret(port);
            paramVal = expression->calculate();
            delete interpreter;
            delete expression;
            port = to_string(paramVal);
        } catch (const char* e) {
            if (expression != nullptr) {
                delete expression;
            }
            if (interpreter != nullptr) {
                delete interpreter;
            }
        }
    }
    //open thread
    std::thread openServerThread(readWithServer, port);
    openServerThread.detach();
    while (!isThreadDone) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return numOfParams;
}

//This method provides an implementation of executing ConnectCommand.
int ConnectCommand::execute(vector<string> lexer) {
    auto i = lexer.begin();
    int numOfParams = 3;
    string ip = (*(i + 1));
    string port = (*(i + 2));
    double paramVal;
    //Is the port given as an expression (38+324+3434)..
    if (isItExpressionPattern(port)) {
        Interpreter *interpreter = new Interpreter();
        Expression *expression = nullptr;
        try {
            //remove spaces and return the expression after calculation
            std::string::iterator end_pos = std::remove(port.begin(), port.end(), ' ');
            port.erase(end_pos, port.end());
            expression = interpreter->interpret(port);
            paramVal = expression->calculate();
            delete interpreter;
            delete expression;
            port = to_string(paramVal);
        } catch (const char* e) {
            if (expression != nullptr) {
                delete expression;
            }
            if (interpreter != nullptr) {
                delete interpreter;
            }
        }
    }
    //open thread
    std::thread connectThread(readWithClient, ip, port);
    connectThread.detach();
    return numOfParams;
}

//This method provides an implementation of executing SleepCommand.
int SleepCommand::execute(vector<string> lexer) {
    auto i = lexer.begin();
    string param = (*(i + 1));
    double paramVal;
    int numOfParams = 2, toSleep = 0;
    //Is the amount of time needed to sleep given as an expression (38+324+3434)..
    if (isItExpressionPattern(param)) {
        Interpreter *interpreter = new Interpreter();
        Expression *expression = nullptr;
        try {
            std::string::iterator end_pos = std::remove(param.begin(), param.end(), ' ');
            param.erase(end_pos, param.end());
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
        //if not expression we convert to int
        toSleep = convertStringToInt(param);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(toSleep));
    return numOfParams;
}

//This method provides an implementation of executing PrintCommand.
int PrintCommand::execute(vector<string> lexer) {
    auto i = lexer.begin();
    int numOfParams = 2, stringPrintFlag = 0;
    string toPrint = (*(i + 1));
    //Is toPrint an Expression? Or is it an existing variable? Or even a string?
    if (toPrint.find('"') != std::string::npos) {
        stringPrintFlag = 1;
        std::string::iterator end_pos = std::remove(toPrint.begin(), toPrint.end(), '"');
        toPrint.erase(end_pos, toPrint.end());
    }
    //If toPrint doesn't need to be printed as a string
    if (stringPrintFlag == 0) {
        toPrint = convertVarsToVals(toPrint);
        if (isItExpressionPattern(toPrint)) {
            Interpreter *interpreter = new Interpreter();
            Expression *expression = nullptr;
            try {
                //Remove tailing spaces
                std::string::iterator end_pos = std::remove(toPrint.begin(), toPrint.end(), ' ');
                toPrint.erase(end_pos, toPrint.end());
                expression = interpreter->interpret(toPrint);
                toPrint = to_string(expression->calculate());
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
        }
    }
    cout << toPrint << endl;
    return numOfParams;
}

//This method provides an implementation of executing DefineVarCommand.
int DefineVarCommand::execute(vector<string> lexer) {
    auto i = lexer.begin();
    int numOfParams;
    //Does the definition include a sim path?
    if ((*(i + 3)) == "sim") {
        numOfParams = 5;
        string param1 = (*(i + 1)); //name
        string param2 = (*(i + 2)); //arrow
        string param3 = (*(i + 4)); //simPath
        std::string::iterator end_pos = std::remove(param3.begin(), param3.end(), '"');
        param3.erase(end_pos, param3.end());
        auto *varObj = new VarObject(param2, param3, 0);
        //Add the new var to the symbol table
        if (param2 == "->") {
            symbolTable.actOnTxtSymbolTable(param1, varObj);
        } else {
            symbolTable.actOnXmlSymbolTable(param3, varObj);
            symbolTable.actOnTxtSymbolTable(param1, varObj);
        }
    } else {
        //This case matches the line "var h0 = heading"
        numOfParams = 4;
        VarObject* varObject;
        string arrow, simParam;
        float value;
        string param1 = (*(i + 1)); //name
        string nameOfExistingVar = (*(i +3));
        //If this var doesn't exist in symbolTable - it's a good reason to crash the program
        if (!symbolTable.findInTxtMap(nameOfExistingVar)) {
            exit(-1);
        } else {
            //Get all of the parameters needed from the existing variable
            arrow = (*symbolTable.getTxtSymbolTable().at(nameOfExistingVar)).getArrow();
            value = (*symbolTable.getTxtSymbolTable().at(nameOfExistingVar)).getVarValue();
            simParam = (*symbolTable.getTxtSymbolTable().at(nameOfExistingVar)).getSim();
            std::string::iterator end_pos = std::remove(simParam.begin(), simParam.end(), '"');
            simParam.erase(end_pos, simParam.end());
            varObject = new VarObject(arrow, simParam, value);
            //insert the new var to the map
            symbolTable.actOnTxtSymbolTable(param1, varObject);
        }
    }
    return numOfParams;
}

//This method provides an implementation of executing IfCommand.
int IfCommand::execute(vector<string> lexer) {
    auto i = lexer.begin();
    int numOfParams = 1;
    int j = 0;
    string key = lexer.at(j);
    //As long as the last statement of the scope wasn't reached count how many params
    while (key != "}") {
        numOfParams++;
        j++;
        key = lexer.at(j);
    }
    int index = 5;
    int flag = 0;
    string param1 = (*(i + 1));
    string condition = (*(i + 2));
    string param2 = (*(i + 3));
    double param1Val, param2Val;
    param1Val = getParametersValue(param1);
    param2Val = getParametersValue(param2);
    //As long as the statement is actually true
    while(isConditionTrue(param1Val, param2Val, condition)) {
        //remove the first line - no need anymore
        while (index != 0 && flag == 0) {
            auto first = lexer.begin();
            lexer.erase(first);
            index--;
        }
        flag = 1;
        map<string, Command*> commandMap = commandsForLoops();
        string key = lexer.at(index);
        auto first = lexer.cbegin() + index;
        auto last = lexer.cend();
        //sub vector
        vector<string> subLex(first, last);
        //If the last bracket of the if scope was reached we return
        if(key == "}") {
            return numOfParams;
        }
        Command *curr;
        if(key != "openDataServer" && key != "connectControlClient" && key != "Print" && key != "Sleep") {
            curr = commandMap["update"];
        } else {
            curr = commandMap[key];
        }
        if (curr != nullptr) {
            index += (*curr).execute(subLex);
        }
    }
    return numOfParams;
}

//This method provides an implementation of executing WhileCommand.
int WhileCommand::execute(vector<string> lexer) {
    auto i = lexer.begin();
    int numOfParams = 1;
    int j = 0;
    string key = lexer.at(j);
    //As long as the last statement of the scope wasn't reached count how many params
    while (key != "}") {
        numOfParams++;
        j++;
        key = lexer.at(j);
    }
    int index = 5;
    int flag = 0;
    string param1 = (*(i + 1));
    string condition = (*(i + 2));
    string param2 = (*(i + 3));
    double param1Val, param2Val;
    param1Val = getParametersValue(param1);
    param2Val = getParametersValue(param2);
    //As long as the statement is actually true
    while (isConditionTrue(param1Val, param2Val, condition)) {
        //remove the first line - no need anymore
        while (index != 0 && flag == 0) {
            auto first = lexer.begin();
            lexer.erase(first);
            index--;
        }
        flag = 1;
        map<string, Command*> commandMap = commandsForLoops();
        string key = lexer.at(index);
        auto first = lexer.cbegin() + index;
        auto last = lexer.cend();
        //sub vector
        vector<string> subLex(first, last);
        //Is the last statement of the scope reached we continue while the condition is true
        if(key == "}") {
            // initialize to star get lexer from begginning
            index = 0;
            continue;
        }
        Command *curr;
        if(key != "openDataServer" && key != "connectControlClient" && key != "Print" && key != "Sleep") {
            curr = commandMap["update"];
        } else {
            curr = commandMap[key];
        }
        if (curr != nullptr) {
            index += (*curr).execute(subLex);
        }
        //Update the values of the checked variables - for the next run of the while loop (if needed)
        param1Val = getParametersValue(param1);
        param2Val = getParametersValue(param2);
    }
    return numOfParams;
}

//This method provides an implementation of executing UpdateVarCommand
int UpdateVarCommand::execute(vector<string> lexer) {
    int numOfParams = 3;
    float value = 0;
    auto i = lexer.begin();
    string varName = (*(i));
    string valByStr = (*(i + 2));
    double paramVal;
    if (isItExpressionPattern(valByStr)) {
        //convertVarsToVals deals with the case "h0 - heading" and convert the varNames to the matching values
        valByStr = convertVarsToVals(valByStr);
        Interpreter *interpreter = new Interpreter();
        Expression *expression = nullptr;
        try {
            std::string::iterator end_pos = std::remove(valByStr.begin(), valByStr.end(), ' ');
            valByStr.erase(end_pos, valByStr.end());
            expression = interpreter->interpret(valByStr);
            //The updated value is the result of calculating the expression
            paramVal = expression->calculate();
            delete interpreter;
            delete expression;
            value = (float)paramVal;
        } catch (const char* e) {
            if (expression != nullptr) {
                delete expression;
            }
            if (interpreter != nullptr) {
                delete interpreter;
            }
        }
    } else {
        value = stof(valByStr);
    }
    //look for it at the symbol table
    if (!symbolTable.findInTxtMap(varName)) {
        exit(-1);
    }
    else {
        // get the information of this var
        string arrow = (*symbolTable.getTxtSymbolTable().at(varName)).getArrow();
        string simParam = (*symbolTable.getTxtSymbolTable().at(varName)).getSim();
        std::string::iterator end_pos = std::remove(simParam.begin(), simParam.end(), '"');
        simParam.erase(end_pos, simParam.end());
        auto *varObj = new VarObject(arrow, simParam, value);
        symbolTable.actOnTxtSymbolTable(varName, varObj);
        //update the value
        VarObject* varObject = symbolTable.getXmlSymbolTable()[simParam];
        varObject->setVarValue(value);
        //Insert the update to the list of updates. This list will later be sent to the simulator
        string currCommand = "set " + simParam + " " + to_string(value);
        progMutex.lock();
        sendToSim.push_front(currCommand);
        progMutex.unlock();
    }
    return numOfParams;
}

//This method accepts a string, checks if it contains any varname. If so, it replaces the varname in the matching value.
string Command::convertVarsToVals(string expStr) {
    string token, valsExp, temp;
    list<string> splitList;
    size_t pos, len = 1, operFound, matchFound = 0, begin = 0;
    for (pos = 0; pos < expStr.length(); pos++) {
        matchFound = 0;
        token = expStr.substr(pos, len);
        //Check if one of the operators is within the token.
        //If so, strip it, push the var to the list, and push the operator as well.
        operFound = token.find('+');
        if (operFound != string::npos) {
            len = 1;
            matchFound++;
            if (token.length() != 1) {
                splitList.push_back(token.substr(begin, (token.length() - 1)));
            }
            splitList.emplace_back("+");
        }
        operFound = token.find('-');
        if (operFound != string::npos) {
            len = 1;
            matchFound++;
            if (token.length() != 1) {
                splitList.push_back(token.substr(begin, (token.length() - 1)));
            }
            splitList.emplace_back("-");
        }
        operFound = token.find('/');
        if (operFound != string::npos) {
            len = 1;
            matchFound++;
            if (token.length() != 1) {
                splitList.push_back(token.substr(begin, (token.length() - 1)));
            }
            splitList.emplace_back("/");
        }
        operFound = token.find('*');
        if (operFound != string::npos) {
            len = 1;
            matchFound++;
            if (token.length() != 1) {
                splitList.push_back(token.substr(begin, (token.length() - 1)));
            }
            splitList.emplace_back("*");
        }
        operFound = token.find('(');
        if (operFound != string::npos) {
            len = 1;
            matchFound++;
            if (token.length() != 1) {
                splitList.push_back(token.substr(begin, (token.length() - 1)));
            }
            splitList.emplace_back("(");
        }
        operFound = token.find(')');
        if (operFound != string::npos) {
            len = 1;
            matchFound++;
            if (token.length() != 1) {
                splitList.push_back(token.substr(begin, (token.length() - 1)));
            }
            splitList.emplace_back(")");
        } if (matchFound == 0) {
            pos--;
            len++;
        } else if (token.length() > 1) {
            pos += token.length() - 1;
        }
        if ((pos + len) > expStr.size()) {
            splitList.push_back(token);
            break;
        }
    }
    for (auto i = splitList.begin(); i != splitList.end(); i++) {
        //Remove trailing spaces
        std::string::iterator end_pos = std::remove((*i).begin(), (*i).end(), ' ');
        (*i).erase(end_pos, (*i).end());
        if (isItVarNamePattern((*i))) {
            if (!symbolTable.findInTxtMap((*i))) {
                exit(-1);
            } else {
                //get the value
                string sim = (*symbolTable.getTxtSymbolTable().at((*i))).getSim();
                float valueOfCurrent = (*symbolTable.getXmlSymbolTable().at((sim))).getVarValue();
                valsExp += to_string(valueOfCurrent);
            }
        } else {
            valsExp += (*i);
        }
    }
    return valsExp;
}

//This method converts a given string to an int type
int Command::convertStringToInt(string toConvert) {
    int asInt;
    istringstream iss (toConvert);
    iss >> asInt;
    if (iss.fail()) {
        exit(1);
    }
    return asInt;
}

//This method returns "true" if a given string matches a varname regex, or "false" otherwise.
bool Command::isItVarNamePattern(string potential) {
    regex varNamePattern("^(?![0-9])([a-zA-Z]|_|[0-9])+");
    return regex_match(potential, varNamePattern);
}

//This method returns the value of a certain var that is given as argument.
double ConditionParser::getParametersValue(string param) {
    double paramVal = 0;
    float floatVal = 0;
    if (isItVarNamePattern(param)) {
        if (symbolTable.findInTxtMap(param)) {
            floatVal = (*symbolTable.getTxtSymbolTable().at(param)).getVarValue();
        }
        string simPath = (*symbolTable.getTxtSymbolTable().at(param)).getSim();
        if (symbolTable.findInXmlMap(simPath)) {
            floatVal = (*symbolTable.getXmlSymbolTable().at(simPath)).getVarValue();
        }
        paramVal = (double)floatVal;
    } else if (isItExpressionPattern(param)) {
        auto *interpreter = new Interpreter();
        Expression *expression = nullptr;
        try {
            std::string::iterator end_pos = std::remove(param.begin(), param.end(), ' ');
            param.erase(end_pos, param.end());
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

//This method creates a map that contains all types of possible Commands in a while loop/ if statement.
map<string, Command *> ConditionParser::commandsForLoops() {
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

//This method confirms whether 2 values match the condition provided.
//The supported conditions are - "==","<",">",">=","<=","!="
bool ConditionParser::isConditionTrue(double param1Val, double param2Val, string condition) {
    bool isConditionTrue = false;
    if(condition == "!=") {
        if (param1Val != param2Val) {
            isConditionTrue = true;
        }
    }
    else if(condition == "==") {
        if (abs(param1Val - param2Val) < 0.0001) {
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

//This method returns "true" if a given string matches a Expression regex, or "false" otherwise.
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


