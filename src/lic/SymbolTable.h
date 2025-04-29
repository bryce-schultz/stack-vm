//***********************************************
// SymbolTable.h
//***********************************************

#pragma once

#include <unordered_map>

#include "Symbol.h"

class Scope
{
public:
    void addSymbol(Symbol *symbol);
    Symbol* getSymbol(const std::string& name);
private:
    std::unordered_map<std::string, Symbol*> symbols;
};

class SymbolTable
{
public:
    SymbolTable();
    ~SymbolTable();

    void increaseScope();
    void decreaseScope();
    Symbol *addSymbol(Symbol *symbol);
    Symbol *lookupLocal(const std::string &name);
    Symbol *lookupGlobal(const std::string &name);
private:
    std::vector<Scope> scopes;

    // for memory management purposes
    // all symbols need to stay alive until 
    // we are done with the symbol table completely, 
    // then all symbols can be deleted, so we keep
    // a list of all symbols here.
    std::vector<Symbol*> allSymbols; 
};