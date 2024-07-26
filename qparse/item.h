#ifndef ITEM_H
#define ITEM_H

#include <vector>
#include "symbol.h"

using namespace std;

class Item {
private:
    Symbol head;
    vector<Symbol> body;
    int dotPos; 
    friend ostream &operator<<(ostream &, const Item &);
public:
    Item();
    Item(const Symbol &, const vector<Symbol> &, int dotPos = 0);
    Item(const Item &);

    ~Item();

    Item &operator=(const Item &);

    const Symbol &getHead() const;
    void setHead(const Symbol &);

    const vector<Symbol> &getBody() const;
    bool setBody(const vector<Symbol> &, int);

    int getDotPos() const;
    bool setDotPos(int);

    bool forward();

    bool operator==(const Item &) const;
};

#endif