#include "grammar.h"

bool Grammar::insertToVector(vector<Symbol> &v, const Symbol &s) {
    for (const Symbol &symbol : v) {
        if (symbol == s)
            return false;
    }
    v.push_back(s);
    return true;
}

vector<Item> Grammar::itemsWithHead(const Symbol &symbol) const {
    vector<Item> v;
    for (const Item &item : items) {
        if (item.getHead() == symbol) {
            v.push_back(item);
        }
    }
    return v;
}

void Grammar::dfs(const Symbol &symbol, map<Symbol, int> &color) {
    color[symbol] = 1;
    vector<Item> v = itemsWithHead(symbol);
    for (const Item &item : v) {
        Symbol firstBodySymbol = item.getBody()[0];
        if (firstBodySymbol.isToken()) {
            insertToVector(first[symbol], firstBodySymbol);
            continue;
        }
        if (color[firstBodySymbol] == 0) {
            dfs(firstBodySymbol, color);
        }
        for (const Symbol &childsFirst : first[firstBodySymbol]) {
            insertToVector(first[symbol], childsFirst);
        }
    }
    color[symbol] = 2;
}

void Grammar::computeFirst() {
    map<Symbol, int> color;
    for (const Symbol &symbol : symbols) {
        if (symbol.isToken())
            continue;
        color[symbol] = 0;
    }
    for (const Symbol &symbol : symbols) {
        if (!symbol.isToken() && color[symbol] == 0) {
            dfs(symbol, color);
        }
    }
}

void Grammar::computeFollow() {
    for (const Symbol &symbol : symbols) {
        if (!symbol.isToken()) {
            follow[symbol] = vector<Symbol>();
        }
    }
    // first pass
    follow[items[0].getHead()].push_back(Symbol("$"));
    // second pass
    for (const Item &item : items) {
        for (int i = 0; i < item.getBody().size() - 1; ++i) {
            Symbol bodySymbol = item.getBody()[i];
            Symbol nextBodySymbol = item.getBody()[i + 1];
            if (nextBodySymbol.isToken()) {
                insertToVector(follow[bodySymbol], nextBodySymbol);
            } else {
                for (const Symbol &s : first[nextBodySymbol]) {
                    insertToVector(follow[bodySymbol], s);
                }
            }
        }
    }
    // third pass
    while (true) {
        bool flag = false;
        for (const Item &item : items) {
            const Symbol &lastBodyItem = item.getBody().back();
            for (const Symbol &symbol : follow[item.getHead()]) {
                flag = flag || insertToVector(follow[lastBodyItem], symbol);
            }
        }
        if (!flag)
            break;
    }
}

Grammar::Grammar() {}

Grammar::Grammar(const vector<Item> &items) {
    this->items = items;
    for (const Item &item : items) {
        insertToVector(symbols, item.getHead());
        for (const Symbol &symbol : item.getBody()) {
            insertToVector(symbols, symbol);
        }
    }
    for (const Symbol &symbol : symbols) {
        if (!symbol.isToken()) {
            first[symbol] = vector<Symbol>();
            follow[symbol] = vector<Symbol>();
        }
    }
    computeFirst();
    computeFollow();
}

Grammar::Grammar(const Grammar &grammar) {
    symbols = grammar.symbols;
    items = grammar.items;
    first = grammar.first;
    follow = grammar.follow;
}

Grammar::~Grammar() {}

Grammar &Grammar::operator=(const Grammar &grammar) {
    symbols = grammar.symbols;
    items = grammar.items;
    first = grammar.first;
    follow = grammar.follow;
    return *this;
}

const vector<Symbol> &Grammar::getSymbols() const {
    return symbols;
}

const vector<Item> &Grammar::getItems() const {
    return items;
}

void Grammar::setItems(const vector<Item> &items) {
    this->items = items;
    first = map<Symbol, vector<Symbol>>();
    follow = map<Symbol, vector<Symbol>>();
    for (const Item &item : items) {
        insertToVector(symbols, item.getHead());
        for (const Symbol &symbol : item.getBody()) {
            insertToVector(symbols, symbol);
        }
    }
    for (const Symbol &symbol : symbols) {
        if (!symbol.isToken()) {
            first[symbol] = vector<Symbol>();
            follow[symbol] = vector<Symbol>();
        }
    }
    computeFirst();
    computeFollow();
}

void Grammar::addItem(const Item &item) {
    items.push_back(item);
    first = map<Symbol, vector<Symbol>>();
    follow = map<Symbol, vector<Symbol>>();
    insertToVector(symbols, item.getHead());
    for (const Symbol &symbol : item.getBody()) {
        insertToVector(symbols, symbol);
    }
    for (const Symbol &symbol : symbols) {
        if (!symbol.isToken()) {
            first[symbol] = vector<Symbol>();
            follow[symbol] = vector<Symbol>();
        }
    }
    computeFirst();
    computeFollow();
}

const vector<Symbol> &Grammar::getFirst(const Symbol &symbol) const {
    return first.at(symbol);
}

const vector<Symbol> &Grammar::getFollow(const Symbol &symbol) const {
    return follow.at(symbol);
} 