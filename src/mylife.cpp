#include <mpi.h>
#include <vector>
#include "port.h"
#include <utility>
#include "Grammar.h"
#include "Message.h"

using namespace std;

struct CellHash {
    size_t operator()(const pair<short, short> &x) const {
        return (unsigned short) x.first << 16 | (unsigned) x.second;
    }
};

typedef pair<int, int> Cell;
typedef unordered_map<int, vector<Cell> > INT2VEC;
typedef unordered_map<Cell, int, CellHash> CELL2INT;

void sendMessage(const string &bla, int dest, int tag);

int tagHash(int i, int j);

string receiveMessage(int source, int tag);

void split(vector<string> &tokens, const string &text, char sep);

void loadSentence(string p, vector<string> &terminals);

int main(int argc, char **argv) {
    // Initialize the MPI environment

    Grammar g;
    g.loadFile("data/string_grammar");
    vector<string> terminals;
    loadSentence("data/test_sentence", terminals);
    int N = (int) terminals.size(); // sentence length
//    printf("sentence length : %d\n", N);

    MPI_Init(NULL, NULL);
    int myProcessID;
    MPI_Comm_rank(MPI_COMM_WORLD, &myProcessID);
    int NUM_PROCESS;
    MPI_Comm_size(MPI_COMM_WORLD, &NUM_PROCESS);
    INT2VEC process2stack;
    CELL2INT cell2process;

    if (myProcessID == 0) {
        cout << "Following are the grammar rules :\n";
        g.displayRules();
    }

    bool done = false;
    int d = 0;
    int column = 0;
    int row = 0;
    int counter = 0;
    while (!done) {
        int processID = counter % NUM_PROCESS;
        Cell c = make_pair(row, column);
        process2stack[processID].push_back(c);
        cell2process[c] = processID;
        counter++;
        row++;
        column++;
        if (column == N) {
            row = 0;
            d++;
            column = d;
        }
        if (row == 0 && column == N) {
            done = true;
        }
    }
    vector<Cell> stack = process2stack[myProcessID];

    for (Cell cell : stack) {
        unordered_map<Cell, Message, CellHash> receivedMessages;
        if (cell.first != cell.second) {
            for (int j = cell.first; j < cell.second; j++) { // receive from previous row cells
                int from_processID = cell2process[make_pair(cell.first, j)];
                int tag = tagHash(cell.first, j);
                const string &basic_string = receiveMessage(from_processID, tag);
                Message receivedMsg;
                receivedMsg.fromString(basic_string);
                receivedMessages[receivedMsg.fromCell] = receivedMsg;

            }
            for (int i = cell.second; i > cell.first; i--) {
                int from_processID = cell2process[make_pair(i, cell.second)];
                int tag = tagHash(i, cell.second);
                const string &basic_string = receiveMessage(from_processID, tag);
                Message receivedMsg;
                receivedMsg.fromString(basic_string);
                receivedMessages[receivedMsg.fromCell] = receivedMsg;

            }
        }

        Message sendingMsg;
        sendingMsg.fromCell = cell;
        //string message = to_string(cell.first) + "," + to_string(cell.second) + ",";

        // now compute the sends
        if (cell.first == cell.second) {
            // no wait  need to start sending
            string terminal = terminals[cell.first];
            set<string> nt = g.getLHS(terminal);
            sendingMsg.setNonTerminals(nt);

        } else {
            set<string> setString;

            for (int l = cell.first; l < cell.second; l++) {
                Cell c1 = make_pair(cell.first, l);
                Cell c2 = make_pair(l + 1, cell.second);
                Message msg1 = receivedMessages[c1];
                Message msg2 = receivedMessages[c2];
                for (string s1 : msg1.NonTerminals) {
                    for (string s2 : msg2.NonTerminals) {
                        set<string> vector = g.getLHS(s1, s2);
                        for (string nonTerminal : vector) {
                            setString.insert(nonTerminal);
                        }
                    }
                }
            }

            sendingMsg.setNonTerminals(setString);
            if (cell.first == 0 && cell.second == N - 1) {
                if (setString.find("ROOT") != setString.end()) {
                    cout << "Hurray!! Its a valid sentence!!";
                } else {
                    cout << "Woops!! This sentence sucks!!";
                }
                cout << "\n";
            }
        }
        for (int i = cell.second + 1; i < N; i++) {
            int to_processID = cell2process[make_pair(cell.first, i)];
            int tag = tagHash(cell.first, cell.second);
            sendMessage(sendingMsg.toString(), to_processID, tag);
        }
        for (int i = cell.first - 1; i >= 0; i--) {
            int to_processID = cell2process[make_pair(i, cell.second)];
            int tag = tagHash(cell.first, cell.second);
            sendMessage(sendingMsg.toString(), to_processID, tag);
        }
    }
    MPI_Finalize();

    return 0;
}


string receiveMessage(int source, int tag) {
    MPI::Status status;
    MPI::COMM_WORLD.Probe(source, tag, status);
    int l = status.Get_count(MPI::CHAR);
    char *buf = new char[l];
    MPI::COMM_WORLD.Recv(buf, l, MPI::CHAR, source, tag, status);
    string bla1(buf, l);
    delete[] buf;
    return bla1;
}

int tagHash(int i, int j) {
    return (int) ((unsigned short) i) << 16 | (unsigned) j;
}

void sendMessage(const string &bla, int dest, int tag) {
    MPI::COMM_WORLD.Send(bla.c_str(), bla.length(), MPI::CHAR, dest, tag);
}

void split(vector<string> &tokens, const string &text, char sep) {
    int start = 0, end = 0;

    while ((end = text.find(sep, start)) != string::npos) {
        tokens.push_back(text.substr(start, end - start));
        start = end + 1;
    }
    tokens.push_back(text.substr(start));
}

void loadSentence(string p, vector<string> &terminals) {
    ifstream in(p.c_str());
    if (!in) {
        cerr << "cant load file\n";
        exit(-1);
    }
    string line;
    while (getline(in, line)) {
        split(terminals, line, ' ');
    }
}