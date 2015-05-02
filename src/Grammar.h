//
// Created by Adithya Renduchintala on 5/2/15.
//

#ifndef MYLIFE_GRAMMAR_H
#define MYLIFE_GRAMMAR_H

#include <string>
#include <fstream>
#include <iostream>
#include "port.h"

using namespace std;

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
    Grammar(const string p) {
        ifstream in(p.c_str());
        if (!in) {
            cerr << "cant load file\n";
            exit(-1);
        }
        string line;
        while (getline(in, line)) {

        }
    };

    string getRHS(string lhs1, string lhs2);

    string getRHS(string terminal);
};


#endif //MYLIFE_GRAMMAR_H
