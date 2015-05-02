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


struct StringPairHash {
    size_t operator()(const pair <string, string> &x) const {
        std::hash<std::string> hash_fn;
        size_t x1 = hash_fn(x.first);
        size_t x2 = hash_fn(x.second);
        return (unsigned short) x1 << 16 | (unsigned) x2;
    }
};

using namespace std;
typedef unordered_map<pair<string, string>, string, StringPairHash> RULES;

class Grammar {

public:
/*
 ifstream in(input.c_str());
    if (!in) {
        cerr << "Can't read " << input << endl;
        return;
    }
    d.Convert("|||"); // not needed but original code always converts ||| first
    int lineNum = 0;
    while (getline(in, line)) {
        static vector<unsigned> tmp;
        d.ConvertWhitespaceDelimitedLine(line, &tmp);
        PopulateFeatures(lineNum, tmp, use_null);
        lineNum++;
    }
 */
    Grammar(const string p) { };

    void loadFile(string p);

    vector<string> getLHS(string lhs1, string lhs2);

    vector<string> getLHS(string terminal);

    void split(vector<string> &tokens, const string &text, char sep);

private:
    RULES rules;


};


#endif //MYLIFE_GRAMMAR_H
