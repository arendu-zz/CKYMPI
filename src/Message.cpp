//
// Created by Adithya Renduchintala on 5/3/15.
//

#include "Message.h"

string Message::toString() {
    stringstream s1, s2;
    s1 << fromCell.first;;
    s2 << fromCell.second;
    string message = s1.str() + "," + s2.str() + ",";
    for (string s : NonTerminals) {
        message += s;
        message += ",";
    }
    return message;
}

void Message::fromString(string message) {
    vector<string> rawData;
    split(rawData, message, ',');
    fromCell = make_pair(stoi(rawData[0]), stoi(rawData[1]));
    rawData.erase(rawData.begin() + 0);
    rawData.erase(rawData.begin() + 0);
    rawData.pop_back();
    for (string s : rawData) {
        NonTerminals.insert(s);
    }

}

void Message::setNonTerminals(set<string> nt) {
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