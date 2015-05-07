#ifndef MYLIFE_MESSAGE_H
#define MYLIFE_MESSAGE_H

#include <vector>
#include <set>
#include <sstream>


using namespace std;
typedef pair<int, int> Cell;

class Message {
public:
    set<string> NonTerminals;
    Cell fromCell;


    Message() { };

    Message(Cell cell, set<string> nonterminals) {
        fromCell = cell;
        NonTerminals = nonterminals;
    };

    void setNonTerminals(set<string> nt);

    void split(vector<string> &tokens, const string &text, char sep);

    void fromString(string message);

    string toString();

};


#endif //MYLIFE_MESSAGE_H
