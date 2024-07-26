#ifndef AUTOMATON_H
#define AUTOMATON_H

#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include "state.h"
#include "grammar.h"

using namespace std;

class Automaton {
private:
    Grammar g;
    vector<const State *> states;
    
    map<pair<int, string>, const State *> stateTable;
    map<pair<int, string>, int> ruleTable;
    map<pair<int, string>, string> actionTable;

    const State *alreadyAdded(const State *) const;
    const State *findNextState(const State *, const Symbol &) const;
    void buildAutomaton(const State *);
    friend ostream &operator<<(ostream &, const Automaton &);
public:
    Automaton(const State *, const Grammar &);
    ~Automaton();

    const vector<const State *> &getStates() const;

    const map<pair<int, string>, const State *> &getStateTable() const;
    const map<pair<int, string>, int> &getRuleTable() const;
    const map<pair<int, string>, string> &getActionTable() const;
};

#endif