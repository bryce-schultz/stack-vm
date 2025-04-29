//***********************************************
// SymbolTable.cpp
//***********************************************

#include "SymbolTable.h"

void Scope::addSymbol(Symbol *symbol)
{
    if (!symbol)
    {
        printf("Symbol is null %s:%d\n", __FILE__, __LINE__);
        return;
    }

    symbols[symbol->getName()] = symbol;
}

Symbol *Scope::getSymbol(const std::string &name)
{
    auto it = symbols.find(name);
    if (it != symbols.end())
    {
        return it->second;
    }
    return nullptr;
}

SymbolTable::SymbolTable()
{
    // create the global scope
    increaseScope();
}

SymbolTable::~SymbolTable()
{
    for (Symbol *symbol : allSymbols)
    {
        delete symbol;
    }
}

void SymbolTable::increaseScope()
{
    scopes.push_back(Scope());
}

void SymbolTable::decreaseScope()
{
    scopes.pop_back();
}

Symbol *SymbolTable::addSymbol(Symbol *symbol)
{
    scopes.back().addSymbol(symbol);
    allSymbols.push_back(symbol);

    return symbol;
}

Symbol *SymbolTable::lookupLocal(const std::string &name)
{
    return scopes.back().getSymbol(name);
}

Symbol *SymbolTable::lookupGlobal(const std::string &name)
{
    for (int i = scopes.size() - 1; i >= 0; i--)
    {
        Symbol *symbol = scopes[i].getSymbol(name);
        if (symbol)
        {
            return symbol;
        }
    }
    return nullptr;
}
