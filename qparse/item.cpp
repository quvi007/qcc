#include "item.h"

Item::Item() : Item(Symbol(), vector<Symbol>(), 0) {}

Item::Item(const Symbol &head, const vector<Symbol> &body, int dotPos) {
    if (dotPos > body.size()) {
        cerr << "invalid dot pos\n";
        return;
    }
    this->head = head;
    this->body = body;
    this->dotPos = dotPos;
}

Item::Item(const Item &item) {
    head = item.head;
    body = item.body;
    dotPos = item.dotPos;
}

Item::~Item() {}

Item &Item::operator=(const Item &item) {
    head = item.head;
    body = item.body;
    dotPos = item.dotPos;
    return *this;
}

const Symbol &Item::getHead() const {
    return head;
}

void Item::setHead(const Symbol &head) {
    this->head = head;
}

const vector<Symbol> &Item::getBody() const {
    return body;
}
    
bool Item::setBody(const vector<Symbol> &body, int dotPos) {
    if (dotPos > body.size()) {
        cerr << "invalid dot pos\n";
        return false;
    }
    this->body = body;
    this->dotPos = dotPos;
    return true;
}

int Item::getDotPos() const {
    return dotPos;
}

bool Item::setDotPos(int dotPos) {
    if (dotPos > body.size()) {
        cerr << "invalid dot pos\n";
        return false;
    }
    this->dotPos = dotPos;
    return true;
}

bool Item::forward() {
    if (dotPos < body.size()) {
        dotPos++;
        return true;
    }
    return false;
}

bool Item::operator==(const Item &item) const {
    return head == item.head && body == item.body && dotPos == item.dotPos;
}

ostream &operator<<(ostream &os, const Item &item) {
    os << item.head << " : ";
    int k = 0;
    for (const Symbol &symbol : item.body) {
        if (k == item.dotPos) {
            os << ".";
        }
        os << symbol;
        k++;
    }
    if (k == item.dotPos) {
        os << ".";
    }
    return os;
}