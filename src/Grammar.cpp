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
        LhsStruct lhsStruct;
        lhsStruct.lhs = items[1];
        lhsStruct.score = stod(items[0]);
        lhsStruct.subtree = "";
        //ignoring item[0]
        if (items.size() == 3) {
            vector<string> lhs;
            split(lhs, items[2], ' ');
            if (lhs.size() == 2) {
                addRule(lhsStruct, lhs[0], lhs[1]);

            } else if (lhs.size() == 1) {
                addRule(lhsStruct, lhs[0]);

            }

        } else {
            //this should not happen
        }
    }
}

set<LhsStruct> Grammar::getLHS(string rhs1, string rhs2) {
    set<LhsStruct> lhsSet = rules[make_pair(rhs1, rhs2)];
    //vector<LhsStruct> v(s.begin(), s.end());
    return lhsSet;
}

set<LhsStruct> Grammar::getLHS(string terminal) {
    set<LhsStruct> lhsSet = rules[make_pair(terminal, EPS)];
    return lhsSet;
}

void Grammar::addRule(LhsStruct lhs, string rhs1, string rhs2) {
    rules[make_pair(rhs1, rhs2)].insert(lhs);

}

void Grammar::addRule(LhsStruct lhs, string terminal) {
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
        set<LhsStruct> possibleLhs = kv.second;
        for (LhsStruct lhs: possibleLhs) {
            cout << lhs.lhs << "-->" << rhs.first << "\t" << rhs.second << "(" << lhs.score << ")\n";
        }
    }
}