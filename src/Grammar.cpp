//
// Created by Adithya Renduchintala on 5/2/15.
//

#include <vector>
#include "Grammar.h"


using namespace std;


void Grammar::loadFile(string p) {
    ifstream in(p.c_str());
    if (!in) {
        cerr << "cant load file\n";
        exit(-1);
    }
    string line;

    while (getline(in, line)) {
        vector<string> items;
        split(items, line, '\t');
        //ignoring item[0]
        if (items.size() == 3) {
            vector<string> lhs;
            split(lhs, items[2], ' ');
            if (lhs.size() == 2) {
                addRule(items[1], lhs[0], lhs[1]);

            } else if (lhs.size() == 1) {
                addRule(items[1], lhs[0]);

            }

        } else {
            //this should not happen
        }
    }
}

set<string> Grammar::getLHS(string rhs1, string rhs2) {
    return rules[make_pair(rhs1, rhs2)];
}

set<string> Grammar::getLHS(string terminal) {
    return rules[make_pair(terminal, EPS)];
}

void Grammar::addRule(string lhs, string rhs1, string rhs2) {
    rules[make_pair(rhs1, rhs2)].insert(lhs);
}

void Grammar::addRule(string lhs, string terminal) {
    rules[make_pair(terminal, EPS)].insert(lhs);
}

void Grammar::split(vector<string> &tokens, const string &text, char sep) {
    int start = 0, end = 0;

    while ((end = text.find(sep, start)) != string::npos) {
        tokens.push_back(text.substr(start, end - start));
        start = end + 1;
    }
    tokens.push_back(text.substr(start));
}

void Grammar::displayRules() {
    for (auto kv : rules) {
        pair<string, string> rhs = kv.first;
        set<string> possibleLhs = kv.second;
        for (string lhs: possibleLhs) {
            cerr << lhs << "-->" << rhs.first << "\t" << rhs.second << "\n";
        }
    }
}