#ifndef MYLIFE_MESSAGE_H
#define MYLIFE_MESSAGE_H

#include <vector>
#include <sstream>


using namespace std;
typedef pair<int, int> Cell;

class Message {
public:
    vector<string> NonTerminals;
    vector<string> SubTrees;
    Cell fromCell;

    Message() { };

    Message(Cell cell) {
        fromCell = cell;
    };

    void setNonTerminalsAndParse(vector<string> nt, vector<string> subparse);

    void split(vector<string> &tokens, const string &text, char sep);

    void fromString(string message);


    string toString();


};


#endif //MYLIFE_MESSAGE_H
