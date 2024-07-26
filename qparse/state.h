#ifndef STATE_H
#define STATE_H

#include <vector>
#include <set>
#include "item.h"
#include "grammar.h"

using namespace std;

class State {
private:
    int id;
    vector<Item> items;
    void closure(const Grammar &);
    bool isAlreadyAdded(const Item &) const;
    void dfs(const Item &, const Grammar &);
    friend ostream &operator<<(ostream &, const State &);
public:
    State();
    State(int, const vector<Item> &, const Grammar &);
    State(const State &);

    ~State();

    State &operator=(const State &);

    int getId() const;
    void setId(int);

    const vector<Item> &getItems() const;
    void setItems(const vector<Item> &);

    bool operator==(const State &) const;
};

#endif