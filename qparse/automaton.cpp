#include "automaton.h"

Automaton::Automaton(const State *startingState, const Grammar &g) {
    this->g = g;
    states.push_back(startingState);
    buildAutomaton(startingState);
}

Automaton::~Automaton() {
    for (const State *state : states) {
        delete state;
    }
}

const State *Automaton::alreadyAdded(const State *u) const {
    for (const State *state : states) {
        if (*state == *u) {
            return state;
        }
    }
    return nullptr;
}

const State *Automaton::findNextState(const State *v, const Symbol &symbol) const {
    vector<Item> nextItems;
    for (const Item &item : v->getItems()) {
        int dotPos = item.getDotPos();
        if (dotPos >= item.getBody().size()) {
            continue;
        }
        if (item.getBody()[dotPos] == symbol) {
            Item nextItem = item;
            nextItem.forward();
            nextItems.push_back(nextItem);
        }
    }
    if (nextItems.size() == 0)
        return nullptr;

    return new State(states.size(), nextItems, g);
}

void Automaton::buildAutomaton(const State *startingState) {
    queue<const State *> q;
    q.push(startingState);

    while (!q.empty()) {
        const State *v = q.front();
        q.pop();
        for (const Item &item : v->getItems()) {
            int dotPos = item.getDotPos();
            if (dotPos >= item.getBody().size()) {
                int grammarItemIdx = 0;
                for (const Item &grammarItem : g.getItems()) {
                    if (grammarItem.getHead() == item.getHead() && grammarItem.getBody() == item.getBody())
                        break;
                    grammarItemIdx++;
                }
                for (const Symbol &symbol : g.getFollow(item.getHead())) {
                    pair<int, string> key = make_pair(v->getId(), symbol.getName());
                    stateTable[key] = nullptr;
                    ruleTable[key] = grammarItemIdx;
                    actionTable[key] = "r";
                }
                continue;
            }
            Symbol symbol = item.getBody()[dotPos];
            pair<int, string> key = make_pair(v->getId(), symbol.getName());

            if (stateTable[key] != nullptr) {
                continue;
            }

            const State *u = findNextState(v, symbol);
            if (!u) {
                stateTable[key] = nullptr;
                ruleTable[key] = -1;
                actionTable[key] = "";
                continue;
            }

            const State *temp = alreadyAdded(u);

            if (!temp) {
                states.push_back(u);
                q.push(u);
                temp = u;
            }

            stateTable[key] = temp;
            ruleTable[key] = -1;
            actionTable[key] = (symbol.isToken() ? "s" : "g");
        }
    }
}

const vector<const State *> &Automaton::getStates() const {
    return states;
}

const map<pair<int, string>, const State *> &Automaton::getStateTable() const {
    return stateTable;
}

const map<pair<int, string>, int> &Automaton::getRuleTable() const {
    return ruleTable;
}

const map<pair<int, string>, string> &Automaton::getActionTable() const {
    return actionTable;
}

ostream &operator<<(ostream &os, const Automaton &automaton) {
    for (const State *state : automaton.states) {
        os << *state << "\n";
    }
    return os;
}