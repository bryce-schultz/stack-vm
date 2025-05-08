#include "DeclNode.h"
#include "Symbol.h"

const Token DeclNode::getToken() const
{
    if (symbol)
    {
        return symbol->getToken();
    }
    return TOKEN_NONE;
}