#ifndef MYLIFE_GRAMMAR_H
#define MYLIFE_GRAMMAR_H

#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <iostream>
#include "port.h"

using namespace std;

struct StringPairHash {
    size_t operator()(const pair<string, string> &x) const {
        std::hash<std::string> hash_fn;
        size_t x1 = hash_fn(x.first);
        size_t x2 = hash_fn(x.second);
        return (unsigned short) x1 << 16 | (unsigned) x2;
    }
};

struct LhsStruct {
    string lhs;
    double score;
    string subtree;

    bool operator<(const LhsStruct &other) const {
        return lhs.compare(other.lhs) < 0;
    }

    bool operator==(const LhsStruct &other) const {
        return (lhs.compare(other.lhs) == 0);
    }

};

typedef unordered_map<pair<string, string>, set<LhsStruct>, StringPairHash> RULES;

class Grammar {

public:
    Grammar() { };

    void loadFile(string file);

    set<LhsStruct> getLHS(string rhs1, string rhs2);

    set<LhsStruct> getLHS(string terminal);

    void split(vector<string> &tokens, const string &text, char sep);

    void addRule(LhsStruct lhsScore, string terminal);

    void addRule(LhsStruct lhsScore, string rhs1, string rhs2);

    string EPS = "<eps>";

    void displayRules();

private:
    RULES rules;


};


#endif //MYLIFE_GRAMMAR_H
