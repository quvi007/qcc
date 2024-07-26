#include "token.h"

Token::Token() : Token("", "") {}

Token::Token(const string &name) : Token(name, "") {}

Token::Token(const string &name, const string &attr) : Symbol(name) {
    tokenSymbol = true;
    this->attr = attr;
}

Token::Token(const Token &token) : Symbol(token.name) {
    tokenSymbol = token.tokenSymbol;
    attr = token.attr;
}

Token::~Token() {}

Token &Token::operator=(const Token &token) {
    tokenSymbol = token.tokenSymbol;
    name = token.name;
    attr = token.attr;
    return *this;
}

const string &Token::getAttr() const {
    return attr;
}

void Token::setAttr(const string &attr) {
    this->attr = attr;
}

bool Token::operator==(const Token &token) const {
    return name == token.name && attr == token.attr;
}

ostream &operator<<(ostream &os, const Token &token) {
    os << "<" << token.name << ((token.attr == "") ? "" : ", " + token.attr) << ">";
    return os;
}