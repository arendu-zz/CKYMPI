#ifndef MYLIFE_MESSAGE_H
#define MYLIFE_MESSAGE_H

#include <vector>
#include <sstream>
#include "Grammar.h"


using namespace std;
typedef pair<int, int> Cell;

class Message {
public:
    set<LhsStruct> NonTerminals;
    Cell fromCell;

    Message() { };

    Message(Cell cell) {
        fromCell = cell;
    };

    void setNonTerminalsAndParse(set<LhsStruct> nt);

    void split(vector<string> &tokens, const string &text, char sep);

    void fromString(string message);


    string toString();


};


#endif //MYLIFE_MESSAGE_H
