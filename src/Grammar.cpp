//
// Created by Adithya Renduchintala on 5/2/15.
//

#include <vector>
#include "Grammar.h"


using namespace std;

void Grammar::Grammar(const string p) {
}

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


    }
}

vector<string> Grammar::getLHS(string lhs1, string lhs2) {
    vector<string> v;
    return v;
}

vector<string> Grammar::getLHS(string terminal) {
    vector<string> v;
    return v;
}

void Grammar::split(vector<string> &tokens, const string &text, char sep) {
    int start = 0, end = 0;

    while ((end = text.find(sep, start)) != string::npos) {
        tokens.push_back(text.substr(start, end - start));
        start = end + 1;
    }
    tokens.push_back(text.substr(start));
}
