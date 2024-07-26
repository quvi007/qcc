#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <vector>
#include <map>
#include "token.h"
#include "symbol.h"
#include "item.h"

class Grammar {
private:
    vector<Symbol> symbols;
    vector<Item> items;
    map<Symbol, vector<Symbol>> first;
    map<Symbol, vector<Symbol>> follow;

    bool insertToVector(vector<Symbol> &v, const Symbol &s);
    vector<Item> itemsWithHead(const Symbol &) const;
    void dfs(const Symbol &, map<Symbol, int> &);
    void computeFirst();
    void computeFollow();
public:
    Grammar();
    Grammar(const vector<Item> &);
    Grammar(const Grammar &);
    ~Grammar();

    Grammar &operator=(const Grammar &);

    const vector<Symbol> &getSymbols() const;

    const vector<Item> &getItems() const;
    void setItems(const vector<Item> &);
    void addItem(const Item &);

    const vector<Symbol> &getFirst(const Symbol &) const;
    const vector<Symbol> &getFollow(const Symbol &) const;
};

#endif