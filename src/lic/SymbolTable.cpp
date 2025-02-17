#include "SymbolTable.h"

void Scope::addSymbol(const std::string &name, Symbol *symbol)
{
    symbols[name] = symbol;
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

void SymbolTable::increaseScope()
{
    scopes.push_back(Scope());
}

void SymbolTable::decreaseScope()
{
    scopes.pop_back();
}

void SymbolTable::addSymbol(const std::string &name, Symbol *symbol)
{
    scopes.back().addSymbol(name, symbol);
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
