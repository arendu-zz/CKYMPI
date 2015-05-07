#include "Message.h"
#include <assert.h>
#include <iostream>

using namespace std;

string Message::toString() {
    stringstream s1, s2;
    s1 << fromCell.first;;
    s2 << fromCell.second;
    string message = s1.str() + "," + s2.str() + "|";
    for (int i = 0; i < NonTerminals.size(); i++) {
        message += NonTerminals[i];
        string c = i == NonTerminals.size() - 1 ? "" : ",";
        message += c;
    }

    message += '|';
    for (int i = 0; i < SubTrees.size(); i++) {
        message += SubTrees[i];
        string c = i == NonTerminals.size() - 1 ? "" : ",";
        message += c;
    }

    return message;
};

void Message::fromString(string message) {
    vector<string> rawData;
    split(rawData, message, '|');
    vector<string> cellparts;
    split(cellparts, rawData[0], ',');
    fromCell = make_pair(stoi(cellparts[0]), stoi(cellparts[1]));
    vector<string> ntparts;
    split(ntparts, rawData[1], ',');
    for (string nt : ntparts) {
        NonTerminals.push_back(nt);
    }
    vector<string> subparseparts;
    split(subparseparts, rawData[2], ',');
    for (string sp: subparseparts) {
        SubTrees.push_back(sp);
    }
    assert(NonTerminals.size() == SubTrees.size());
}

void Message::setNonTerminalsAndParse(vector<string> nt, vector<string> st) {
    assert(nt.size() == st.size());
    NonTerminals.clear();
    SubTrees.clear();
    for (int i = 0; i < nt.size(); i++) {
        string n = nt[i];
        string p = st[i];
        if (std::find(NonTerminals.begin(), NonTerminals.end(), n) == NonTerminals.end()) {
            NonTerminals.push_back(n);
            SubTrees.push_back(p);
        }
    }


}

void Message::split(vector<string> &tokens, const string &text, char sep) {
    int start = 0, end = 0;

    while ((end = text.find(sep, start)) != string::npos) {
        tokens.push_back(text.substr(start, end - start));
        start = end + 1;
    }
    tokens.push_back(text.substr(start));
}