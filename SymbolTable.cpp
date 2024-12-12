#include <bits/stdc++.h>

using namespace std;

class Parameter {
private:
    string name, type;
public:
    Parameter() : Parameter("", "") {}
    
    Parameter(const string &name, const string &type) {
        this->name = name;
        this->type = type;
    }

    string getName() const {
        return name;
    }

    void setName(const string &name) {
        this->name = name;
    }

    string getType() const {
        return type;
    }

    void setType(const string &type) {
        this->type = type;
    }
};

class SymbolInfo {
private:
    string name, type;
    SymbolInfo *next;
    int size;
    bool defined;
    vector<Parameter> parameterList;
    int offset;
public:
    SymbolInfo() : SymbolInfo("", "") {}
    SymbolInfo(const string &name, const string &type) {
        this->name = name;
        this->type = type;
        next = nullptr;
        size = 0;
        defined = false;
    }
    SymbolInfo(const SymbolInfo &symbolInfo) {
        name = symbolInfo.name;
        type = symbolInfo.type;
        next = symbolInfo.next;
        size = symbolInfo.size;
        parameterList = symbolInfo.parameterList;
        defined = symbolInfo.defined;
        offset = symbolInfo.offset;
    }
    ~SymbolInfo() {}
    const string &getName() const {
        return name;
    }
    void setName(const string &name) {
        this->name = name;
    }
    const string &getType() const {
        return type;
    }
    void setType(const string &type) {
        this->type = type;
    }
    SymbolInfo *getNext() const {
        return next;
    }
    void setNext(SymbolInfo* symbolInfo) {
        next = symbolInfo;
    }
    int getSize() const {
        return size;
    }
    void setSize(int size) {
        this->size = size;
    }
    bool isDefined() const {
        return defined;
    }
    void setDefined(bool defined) {
        this->defined = defined;
    }
    vector<Parameter> getParameterList() const {
        return parameterList;
    }
    void addParameter(const string &name, const string &type) {
        parameterList.push_back(Parameter(type, name));
    }
    int getParameterListSize() const {
        return parameterList.size();
    }
    void setAsArray(const string &name, const string &type, int size) {
        this->name = name;
        this->type = type;
        next = nullptr;
        this->size = size;
        defined = false;
    }
    void setAsFunction(const string &name, const string &type, const vector<Parameter> &parameterList) {
        this->name = name;
        this->type = type;
        size = -1;
        this->parameterList = parameterList;
    }
    
    bool isVariable() const {
        return size == 0;
    }

    bool isArray() const {
        return size > 0;
    }

    bool isFunction() const {
        return size == -1;
    }

    void setOffset(int offset) {
        this->offset = offset;
    }

    int getOffset() const {
        return offset;
    }
};

class ScopeTable {
private:
    static unsigned long sdbmhash(const string &str) {
        unsigned long hash = 0;
        int c, k = 0;
        while ((c = str[k++]))
            hash = c + (hash << 6) + (hash << 16) - hash;
        return hash;
    }

    string id;
    int total_buckets;
    SymbolInfo **T;
    ScopeTable* parentScope;
    int childCount;

    int localOffset;
    
    int variableCount;
public:
    ScopeTable(int n, ScopeTable *parentScope) : total_buckets{n}, parentScope{parentScope}, childCount{0}, localOffset{0}, variableCount{0} {
        T = new SymbolInfo* [n];
        for (int i = 0; i < n; ++i)
            T[i] = nullptr;
        if (parentScope) {
            id = parentScope->id + "." + to_string(parentScope->childCount + 1);
            parentScope->childCount = parentScope->childCount + 1;
        }
        else id = "1";
    }

    ~ScopeTable() {
        for (int i = 0; i < total_buckets; ++i) {
            SymbolInfo *p = T[i];
            while (p) {
                SymbolInfo *next = p->getNext();
                delete p;
                p = next;
            }
        }
        delete[] T;
    }

    SymbolInfo *lookUp(const string &symbolName) const {
        int idx = sdbmhash(symbolName) % total_buckets;
        SymbolInfo *p = T[idx];
        int c = 0;
        while (p) {
            if (p->getName() == symbolName) {
                return p;
            }
            p = p->getNext();
            c++;
        }
        return nullptr;
    }

    void print() const {
        cout << "\nScopeTable # " << id << "\n";
        for (int i = 0; i < total_buckets; ++i) {
            if (T[i] == nullptr) continue;
            cout << i << " -->";
            SymbolInfo *p = T[i];
            while (p) {
                cout << "  < " << p->getName() << " : " << p->getType() << ">";
                p = p->getNext();
            }
            cout << "\n";
        }
        cout << "\n";
    }

    void print(FILE *fp) const {
        fprintf(fp, "\nScopeTable # %s\n", id.c_str());
        for (int i = 0; i < total_buckets; ++i) {
            if (T[i] == nullptr) continue;
            fprintf(fp, "%d -->", i);
            SymbolInfo *p = T[i];
            while (p) {
                fprintf(fp, "  < %s , %s>", p->getName().c_str(), p->getType().c_str());
                p = p->getNext();
            }
            fprintf(fp, "\n");
        }
        fprintf(fp, "\n");
    }

    ScopeTable *getParentScope() const {
        return parentScope;
    }

    void setParentScope(ScopeTable *scopeTable) {
        parentScope = scopeTable;
    }

    bool insertEntry(const string &symbolName, const string &symbolType) {
        int idx = sdbmhash(symbolName) % total_buckets;
        int c = 0;
        if (T[idx]) {
            SymbolInfo *p = T[idx], *prev = nullptr;
            while (p) {
                if (p->getName() == symbolName) {
                    // cout << "\n" << symbolName << " already exists in current ScopeTable\n";
                    return false;
                }
                prev = p;
                p = p->getNext();
                c++;
            }
            SymbolInfo *symbolInfo = new SymbolInfo(symbolName, symbolType);
            prev->setNext(symbolInfo);
        } else T[idx] = new SymbolInfo(symbolName, symbolType);
        return true;
    }

    bool insertEntry(const SymbolInfo &symbolInfo) {
        string symbolName = symbolInfo.getName();
        int idx = sdbmhash(symbolName) % total_buckets;
        int c = 0;
        if (T[idx]) {
            SymbolInfo *p = T[idx], *prev = nullptr;
            while (p) {
                if (p->getName() == symbolName) {
                    // cout << "\n" << symbolName << " already exists in current ScopeTable\n";
                    return false;
                }
                prev = p;
                p = p->getNext();
                c++;
            }
            prev->setNext(new SymbolInfo(symbolInfo));
        } else T[idx] = new SymbolInfo(symbolInfo);
        return true;
    }

    bool deleteEntry(const string &symbolName) const {
        int idx = sdbmhash(symbolName) % total_buckets;
        SymbolInfo *p = T[idx], *prev = nullptr;
        int c = 0;
        while (p) {
            if (p->getName() == symbolName) {
                SymbolInfo *next = p->getNext();
                if (prev) prev->setNext(next);
                else T[idx] = next;
                delete p;
                return true;
            }
            prev = p;
            p = p->getNext();
            c++;
        }
        return false;
    }

    const string &getId() const {
        return id;
    }

    void setLocalOffset(int localOffset) {
        this->localOffset = localOffset;
    }

    void decreaseLocalOffset(int x) {
        localOffset -= x;
    }

    void increaseLocalOffset(int x) {
        localOffset += x;
    }

    int getLocalOffset() const {
        return localOffset;
    }

    void setVariableCount(int variableCount) {
        this->variableCount = variableCount;
    }

    int getVariableCount() {
        return variableCount;
    }

    void increaseVariableCount(int x) {
        variableCount += x;
    }

    void decreaseVariableCount(int x) {
        variableCount -= x;
    }

};

class SymbolTable {
private:
    int total_buckets;
    ScopeTable *currentScopeTable;
    stack<ScopeTable*> scopeTables;
public:
    SymbolTable(int total_buckets) {
        this->total_buckets = total_buckets;
        currentScopeTable = nullptr;
        enterScope();
    }

    ~SymbolTable() {
        while (!scopeTables.empty()) {
            ScopeTable *scopeTable = scopeTables.top();
            scopeTables.pop();
            delete scopeTable;
        }
    }

    void enterScope() {
        ScopeTable* scopeTable = new ScopeTable(total_buckets, currentScopeTable);
        currentScopeTable = scopeTable;
        scopeTables.push(scopeTable);
    }

    void exitScope() {
        scopeTables.pop();
        delete currentScopeTable;
        currentScopeTable = nullptr;
        if (!scopeTables.empty())
            currentScopeTable = scopeTables.top();
    }

    bool insert(const string &symbolName, const string &symbolType) {
        return currentScopeTable->insertEntry(symbolName, symbolType);
    }

    bool insert(const SymbolInfo &symbolInfo) {
        return currentScopeTable->insertEntry(symbolInfo);
    }

    bool remove(const string &symbolName) {
        return currentScopeTable->deleteEntry(symbolName);
    }

    SymbolInfo *lookUp(const string &symbolName) const {
        ScopeTable *p = currentScopeTable;
        while (p) {
            SymbolInfo* symbolInfo = p->lookUp(symbolName);
            if (symbolInfo) return symbolInfo;
            p = p->getParentScope();
        }
        return nullptr;
    }

    void printCurrentScopeTable() const {
        currentScopeTable->print();
    }

    void printAllScopeTables() const {
        stack<ScopeTable*> temp = scopeTables;
        while (!temp.empty()) {
            temp.top()->print();
            temp.pop();
        }
    }

    void printAllScopeTables(FILE *fp) const {
        stack<ScopeTable*> temp = scopeTables;
        while (!temp.empty()) {
            temp.top()->print(fp);
            temp.pop();
        }
    }

    ScopeTable* getCurrentScopeTable() const {
        return currentScopeTable;
    }

    void setCurrentScopeTable(ScopeTable *scopeTable) {
        currentScopeTable = scopeTable;
    }
};