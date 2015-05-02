//
// Created by Adithya Renduchintala on 5/2/15.
//

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


typedef unordered_map<pair<string, string>, set<string>, StringPairHash> RULES;

class Grammar {

public:
    Grammar() { };

    void loadFile(string p);

    set<string> getLHS(string rhs1, string rhs2);

    set<string> getLHS(string terminal);

    void split(vector<string> &tokens, const string &text, char sep);

    void addRule(string lhs, string terminal);

    void addRule(string lhs, string rhs1, string rhs2);

    string EPS = "<eps>";

    void displayRules();

private:
    RULES rules;


};


#endif //MYLIFE_GRAMMAR_H
