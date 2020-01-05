//
// Created by yael and linoy on 19/12/2019.
//

#include "SymbolTable.h"

//check if the object exist in the txt map
bool SymbolTable::findInTxtMap(string name) {
    return !(this->txtSymbolTable.find(name) == this->txtSymbolTable.end());
}

//check if the object exist in the xml map
bool SymbolTable::findInXmlMap(string name) {
    return !(this->xmlSymbolTable.find(name) == this->xmlSymbolTable.end());
}

// insert object to the map
void SymbolTable::actOnTxtSymbolTable(string name, VarObject* varObj) {
    // if exist we update the values in the map if not we insert it to the map
    if (this->txtSymbolTable.find(name) == this->txtSymbolTable.end()) {
        this->txtSymbolTable.insert({name, varObj});
    } else {
        this->txtSymbolTable.at(name)->setVarValue((*varObj).getVarValue());
    }
}

// insert object to the map
void SymbolTable::actOnXmlSymbolTable(string name, VarObject* varObj) {
    // if exist we update the values in the map if not we insert it to the map
    if (this->xmlSymbolTable.find(name) == this->xmlSymbolTable.end()) {
        this->xmlSymbolTable.insert({name, varObj});
    } else {
        this->xmlSymbolTable.at(name)->setVarValue((*varObj).getVarValue());
    }
}

// a getter of the txt map
unordered_map<string, VarObject*> SymbolTable::getTxtSymbolTable() {
    return this->txtSymbolTable;
}

// a getter of the xml map
unordered_map<string, VarObject*> SymbolTable::getXmlSymbolTable() {
    return this->xmlSymbolTable;
}