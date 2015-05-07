#include "Message.h"
#include "Grammar.h"
#include <assert.h>
#include <iostream>

using namespace std;

string Message::toString() {
    stringstream s1, s2;
    s1 << fromCell.first;;
    s2 << fromCell.second;

    string message = s1.str() + "," + s2.str() + "|";
    int i = 0;
    for (LhsStruct nt : NonTerminals) {
        stringstream scoreStr;
        scoreStr << nt.score;
        message += nt.lhs + "," + scoreStr.str() + "," + nt.subtree;
        string c = i == NonTerminals.size() - 1 ? "" : "|";
        message += c;
        i++;
    }


    return message;
};

void Message::fromString(string message) {
    vector<string> rawData;
    split(rawData, message, '|');
    vector<string> cellparts;
    split(cellparts, rawData[0], ',');
    fromCell = make_pair(stoi(cellparts[0]), stoi(cellparts[1]));
    NonTerminals.clear();
    for (int i = 1; i < rawData.size(); i++) {
        if (rawData[i].compare("") != 0) {
            LhsStruct nt;
            vector<string> ntparts;
            split(ntparts, rawData[i], ',');
            assert(ntparts.size() == 3);
            nt.lhs = ntparts[0];
            nt.score = stod(ntparts[1]);
            nt.subtree = ntparts[2];
            NonTerminals.insert(nt);
        }

    }

}

void Message::setNonTerminalsAndParse(set<LhsStruct> nt) {
    NonTerminals.clear();
    NonTerminals = nt;
}

void Message::split(vector<string> &tokens, const string &text, char sep) {
    int start = 0, end = 0;

    while ((end = text.find(sep, start)) != string::npos) {
        tokens.push_back(text.substr(start, end - start));
        start = end + 1;
    }
    tokens.push_back(text.substr(start));
}