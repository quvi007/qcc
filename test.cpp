#include <bits/stdc++.h>
#include "qparse/grammar.h"
#include "qparse/item.h"
#include "qparse/state.h"
#include "qparse/symbol.h"
#include "qparse/token.h"

using namespace std;

int main() {
    // grammar 1
    
    Symbol E_p("E\'");
    Symbol E("E");
    Symbol T("T");
    Symbol F("F");
    Token id("ID");
    Token ADDOP("+");
    Token MULOP("*");
    Token LPAREN("(");
    Token RPAREN(")");
    Token EM("$");

    int a = 5 + -6;

    vector<Item> items = {
        Item(E_p, {E}, 0),
        Item(E, {E, ADDOP, T}, 0),
        Item(E, {T}, 0),
        Item(T, {T, MULOP, F}, 0),
        Item(T, {F}, 0),
        Item(F, {LPAREN, E, RPAREN}, 0),
        Item(F, {id}, 0)
    };
    
    vector<Symbol> symbols = {
        E_p, E, T, F, id, ADDOP, MULOP, LPAREN, RPAREN
    };

    // grammar 2
    /*
    Symbol L_p("L\'");
    Symbol L("L");
    Symbol T("T");
    Token LPAREN("(");
    Token RPAREN(")");
    Token EM("$");

    vector<Item> items = {
        Item(L_p, {L}, 0),
        Item(L, {L, T}, 0),
        Item(L, {T}, 0),
        Item(T, {LPAREN, L, RPAREN}, 0),
        Item(T, {LPAREN, RPAREN}, 0)
    };
    
    vector<Symbol> symbols = {
        L_p, L, T, LPAREN, RPAREN
    };
    */
    //

    Grammar g(symbols, items);
    for (const Symbol &symbol : symbols) {
        if (!symbol.isToken()) {
            cout << "FOLLOW(" << symbol.getName() << "): {";
            int sz = g.getFollow(symbol).size(), k = 0;
            for (const Symbol &firstSymbol : g.getFollow(symbol)) {
                cout << firstSymbol.getName();
                if (k < sz - 1) {
                    cout << ", ";
                } else cout << "}\n";
                k++;
            }
        }
    }
    return 0;
}