//
// Created by yael on 19/12/2019.
//

#include "SymbolTable.h"

bool SymbolTable::findInTxtMap(string name) {
    return !(this->txtSymbolTable.find(name) == this->txtSymbolTable.end());
}

void SymbolTable::actOnTxtSymbolTable(string name, VarObject varObj) {
    if (this->txtSymbolTable.find(name) == this->txtSymbolTable.end()) {
        this->txtSymbolTable.insert({name, varObj});
    } else {
        //this->txtSymbolTable.emplace(name, varObj);
        this->txtSymbolTable.at(name).setVarValue(varObj.getVarValue());
    }
}

void SymbolTable::actOnXmlSymbolTable(string name, VarObject varObj) {
    if (this->xmlSymbolTable.find(name) == this->xmlSymbolTable.end()) {
        this->xmlSymbolTable.insert({name, varObj});
    } else {
        //this->xmlSymbolTable.emplace(name, varObj);
        this->xmlSymbolTable.at(name).setVarValue(varObj.getVarValue());
    }
}

unordered_map<string, VarObject> SymbolTable::getTxtSymbolTable() {
    return this->txtSymbolTable;
}

unordered_map<string, VarObject> SymbolTable::getXmlSymbolTable() {
    return this->xmlSymbolTable;
}

bool SymbolTable::findInXmlMap(string name) {
    return !(this->xmlSymbolTable.find(name) == this->xmlSymbolTable.end());
}

