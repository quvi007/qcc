#include "symbol.h"

Symbol::Symbol() : Symbol("") {}

Symbol::Symbol(const string &name) {
    this->name = name;
}

Symbol::Symbol(const Symbol &symbol) {
    tokenSymbol = symbol.tokenSymbol;
    name = symbol.name;
}

Symbol::~Symbol() {}

Symbol &Symbol::operator=(const Symbol &symbol) {
    tokenSymbol = symbol.tokenSymbol;
    name = symbol.name;
    return *this;
}

const string &Symbol::getName() const {
    return name;
}

void Symbol::setName(const string &name) {
    this->name = name;
}

bool Symbol::operator==(const Symbol &symbol) const {
    return name == symbol.name && tokenSymbol == symbol.tokenSymbol;
}

bool Symbol::operator<(const Symbol &symbol) const {
    return name < symbol.name;
}

bool Symbol::isToken() const {
    return tokenSymbol;
}

ostream &operator<<(ostream &os, const Symbol &symbol) {
    os << "<" << symbol.name << ">"; 
    return os;
}