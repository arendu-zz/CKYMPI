#include "Message.h"

string Message::toString() {
    stringstream s1, s2;
    s1 << fromCell.first;;
    s2 << fromCell.second;
    string message = s1.str() + "," + s2.str() + "|";
    for (string s : NonTerminals) {
        message += s;
        message += ",";
    }
    /*message += '|';
    for (string p : subtrees) {
        message += p;
        message += ",";
    }*/
    return message;
}

void Message::fromString(string message) {
    vector<string> rawData;
    split(rawData, message, '|');
    vector<string> cellparts;
    split(cellparts, rawData[0], ',');
    fromCell = make_pair(stoi(cellparts[0]), stoi(cellparts[1]));
    vector<string> ntparts;
    split(ntparts, rawData[1], ',');
    for (string nt : ntparts) {
        NonTerminals.insert(nt);
    }
    /*vector<string> subparseparts;
    split(subparseparts, rawData[2], ',');
    for (string sp: subparseparts) {
        subtrees.insert(sp);
    }*/
}

void Message::setNonTerminals(set<string> nt) {
    NonTerminals = nt;

}

void Message::makeBracketedString() {
    if (spanStr.compare("") == 0) {
        //pass this is not a diagonal cell
    } else {
        for (string nt : NonTerminals) {
            string parse = "(" + nt + " " + spanStr + ")";
            subtrees.insert(parse);
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