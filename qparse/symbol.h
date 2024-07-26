#ifndef SYMBOL_H
#define SYMBOL_H

#include <iostream>
#include <string>

using namespace std;

class Symbol {
private:
    friend ostream &operator<<(ostream &, const Symbol &);
protected:
    bool tokenSymbol = false;
    string name;
public:
    Symbol();
    Symbol(const string &);
    Symbol(const Symbol &);

    ~Symbol();

    Symbol &operator=(const Symbol &);

    const string &getName() const;
    void setName(const string &);

    bool operator==(const Symbol &) const;

    bool operator<(const Symbol &) const;

    bool isToken() const;
};

#endif