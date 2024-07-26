#include "state.h"

void State::closure(const Grammar &g) {
    vector<Item> t = items;
    for (const Item &item : t) {
        dfs(item, g);
    }
}

bool State::isAlreadyAdded(const Item &u) const {
    for (const Item &item : items) {
        if (item == u) return true;
    }
    return false;
}

void State::dfs(const Item &v, const Grammar &g) {
    int dotPos = v.getDotPos();
    if (dotPos >= v.getBody().size())   
        return;
    Symbol symbol = v.getBody()[dotPos];
    for (const Item &u : g.getItems()) {
        if (u.getHead() == symbol) {
            if (!isAlreadyAdded(u)) {
                items.push_back(u);
                dfs(u, g);
            }
        }
    }
}

State::State() {}

State::State(int id, const vector<Item> &items, const Grammar &g) {
    this->id = id;
    this->items = items;
    closure(g);
}

State::State(const State &state) {
    id = state.id;
    items = state.items;
}

State::~State() {}

State &State::operator=(const State &state) {
    id = state.id;
    items = state.items;
    return *this;
}

int State::getId() const {
    return id;
}

void State::setId(int id) {
    this->id = id;
}

const vector<Item> &State::getItems() const {
    return items;
}

void State::setItems(const vector<Item> &items) {
    this->items = items;
}

bool State::operator==(const State &state) const {
    bool flag = true;
    for (const Item &item : state.items) {
        if (!isAlreadyAdded(item)) {
            flag = false;
            break;
        }
    }
    return flag && items.size() == state.items.size();
}

ostream &operator<<(ostream &os, const State &state) {
    os << "State " << state.id << ":\n";
    for (const Item &item : state.items) {
        os << item << "\n";
    }
    return os;
}