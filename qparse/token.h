#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>
#include "symbol.h"

using namespace std;

class Token : public Symbol {
private:
    string attr;
    friend ostream &operator<<(ostream &, const Token &);
public:
    Token();
    Token(const string &);
    Token(const string &, const string &);

    Token(const Token &);

    ~Token();

    Token &operator=(const Token &);

    const string &getAttr() const;
    void setAttr(const string &);

    bool operator==(const Token &) const;
};

#endif