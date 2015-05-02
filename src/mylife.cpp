#include <mpi.h>
#include <vector>
#include "port.h"
#include <utility>
#include "Grammar.h"

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

int main(int argc, char **argv) {
    // Initialize the MPI environment

    Grammar g;
    g.loadFile("data/string_grammar");
    string sentence[] = {"a", "b", "c", "d"};

    MPI_Init(NULL, NULL);
    int myProcessID;
    MPI_Comm_rank(MPI_COMM_WORLD, &myProcessID);
    int NUM_PROCESS;
    MPI_Comm_size(MPI_COMM_WORLD, &NUM_PROCESS);
    INT2VEC process2stack;
    CELL2INT cell2process;

    if (myProcessID == 0) {
        g.displayRules();
    }

    int N = 4; // sentence length
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
        printf("%d, cell: %d,%d\n", myProcessID, cell.first, cell.second);

        unordered_map<Cell, vector<string>, CellHash> receivedMessages;
        if (cell.first != cell.second) {
            for (int j = cell.first; j < cell.second; j++) { // receive from previous row cells
                int from_processID = cell2process[make_pair(cell.first, j)];
                int tag = tagHash(cell.first, j);
                const string &basic_string = receiveMessage(from_processID, tag);
                vector<string> rawData;
                split(rawData, basic_string, ',');
                Cell reCell = make_pair(stoi(rawData[0]), stoi(rawData[1]));
                rawData.erase(rawData.begin() + 0);
                rawData.erase(rawData.begin() + 1);
                receivedMessages[reCell] = rawData;
            }
            for (int i = cell.second; i > cell.first; i--) {
                int from_processID = cell2process[make_pair(i, cell.second)];
                int tag = tagHash(i, cell.second);
                const string &basic_string = receiveMessage(from_processID, tag);
                vector<string> rawData;
                split(rawData, basic_string, ',');
                Cell reCell = make_pair(stoi(rawData[0]), stoi(rawData[1]));
                rawData.erase(rawData.begin() + 0);
                rawData.erase(rawData.begin() + 1);
                receivedMessages[reCell] = rawData;
            }
        }

        string message = to_string(cell.first) + "," + to_string(cell.second) + ",";
        // now compute the sends
        if (cell.first == cell.second) {
            // no wait  need to start sending
            string terminal = sentence[cell.first];
            const vector<string> &vector = g->getLHS(terminal);
            for (string s : vector) {
                message += s;
                message += ",";
            }
        } else {
            set<string> setString;

            for (int l = cell.first; l <= cell.second; l++) {
                Cell c1 = make_pair(cell.first, l);
                Cell c2 = make_pair(l + 1, cell.second);
                vector<string> first = receivedMessages[c1];
                vector<string> second = receivedMessages[c2];
                for (string s1 : first) {
                    for (string s2 : second) {
                        const vector<string> &vector = g.getLHS(s1, s2);
                        for (string nonTerminal : vector) {
                            setString.insert(nonTerminal);
                        }
                    }
                }
            }

            for (string nonTerminal : setString) {
                message += nonTerminal;
                message += ",";
            }
        }
        for (int i = cell.second + 1; i < N; i++) {
            int to_processID = cell2process[make_pair(cell.first, i)];
            int tag = tagHash(cell.first, cell.second);
            sendMessage(message, to_processID, tag);
        }
        for (int i = cell.first - 1; i >= 0; i--) {
            int to_processID = cell2process[make_pair(i, cell.second)];
            int tag = tagHash(cell.first, cell.second);
            sendMessage(message, to_processID, tag);
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
    cout << bla1 + "\n";
    return bla1;
}

int tagHash(int i, int j) {
    return i + j;//(int) ((unsigned short) i) << 16 | (unsigned) j;
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
