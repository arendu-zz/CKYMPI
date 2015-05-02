//
// Created by Adithya Renduchintala on 5/2/15.
//

#include <vector>
#include "Grammar.h"


using namespace std;

void Grammar::Grammar(const string p) {
}

void loadFile(string p) {
    ifstream in(p.c_str());
    if (!in) {
        cerr << "cant load file\n";
        exit(-1);
    }
    string line;
    while (getline(in, line)) {

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