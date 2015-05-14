#include <mpi.h>
#include <vector>
#include "port.h"
#include "Grammar.h"
#include "Message.h"
#include <chrono>

using namespace std;
using namespace std::chrono;

// utility method required to enable use of a Cell as key in map.
struct CellHash {
    size_t operator()(const pair<short, short> &x) const {
        return (unsigned short) x.first << 16 | (unsigned) x.second;
    }
};

// Pair of integers, used to represent cell in CKY matrix.
typedef pair<int, int> Cell;
typedef unordered_map<int, vector<Cell> > INT2VEC;
typedef unordered_map<Cell, int, CellHash> CELL2INT;

// Sending message to destination process tagged with given tag.
void sendMessage(const string &message, int dest, int tag);

// This method is responsible for receiving message from given source process and tagged as tag. We are using tags
// while passing messages because we might have to access messages out of order.
string receiveMessage(int source, int tag);

// This method is used to evaluate tag we are assining to our outgoing message. As tag can only be an integer, we are
// hashing cell indices into a single integer.
int tagHash(int i, int j);

// Fills the given vector by spltting text by given seperator.
void split(vector<string> &tokens, const string &text, char sep);

// Given file containing sentence and delmiter used, it loads the sentence.
void loadSentence(string fileName, vector<string> &terminals);

int main(int argc, char **argv) {
    Grammar g;
    g.loadFile("data/string_grammar");
    vector<string> terminals;
    loadSentence("data/test_sentence", terminals);
    int N = (int) terminals.size(); // sentence length

    // Initialize the MPI environment
    MPI_Init(NULL, NULL); // We are not passing any argument from command line, hence (NULL,NULL)
    int myProcessID; // Processid/Rank of this process.
    MPI_Comm_rank(MPI_COMM_WORLD, &myProcessID);
    int NUM_PROCESS; // Total number of processes.
    MPI_Comm_size(MPI_COMM_WORLD, &NUM_PROCESS);
    INT2VEC process2stack; // This tracks all the cell a process is responsible for.
    CELL2INT cell2process; // This tracks which process is responsible for given cell.

    // We are using this for tracking CPU time spent on parsing.
    milliseconds startMs = duration_cast<milliseconds>(
            system_clock::now().time_since_epoch()
    );

    // Here we are doing the initial assignment of process and cell of the CKY matrix.
    // This computation is done by each process, as we require that each process is aware of the mapping between various
    // cells and process responsible for them.
    bool done = false;
    int d = 0;
    int column = 0;
    int row = 0;
    int counter = 0;
    while (!done) {
        // Assigninment of cells to process is done along the diagonal to ensure that maximum number of process can run in parallel.
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

    vector<Cell> stack = process2stack[myProcessID]; // This is the stack of cell required to be processed by this process.

    for (Cell cell : stack) {
        // We will start by processing bottom most element of stack and progressively process cells far from the diagonal.
        unordered_map<Cell, Message, CellHash> receivedMessages;
        if (cell.first != cell.second) {
            // If we are away from the diagonal, we need to receive messages involving non-terminal rewrites from cells closer to the diagonal.
            for (int j = cell.first; j < cell.second; j++) { // receive from previous row cells
                int from_processID = cell2process[make_pair(cell.first, j)];
                int tag = tagHash(cell.first, j);
                const string &message = receiveMessage(from_processID, tag);
                Message receivedMsg;
                receivedMsg.fromString(message);
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

        Message messageToBeSent;
        messageToBeSent.fromCell = cell; // Configuring source cell in message object to be sent.

        // now compute the sends
        if (cell.first == cell.second) {
            // If we are at the diagonal, then it is the initialization. We didn't receive any messages from anywhere.
            // Sending messages related to rewrite rules of terminal symbols.
            string terminal = terminals[cell.first];
            set<LhsStruct> nt;

            for (LhsStruct n : g.getLHS(terminal)) {
                n.subtree = "(" + n.lhs + " " + terminal + ")";
                nt.insert(n);
            }
            messageToBeSent.setNonTerminalsAndParse(nt); // Setting message content in message.
        } else {
            set<LhsStruct> newNonTerminals;

            // We have received all the messages required for processing. Now we are going to process partition ranges and
            // see if a rewrite rule exists or not. Also if two different partitions result in same rule, we are keeping the
            // score of most probable rewrite.
            for (int l = cell.first; l < cell.second; l++) {
                Cell leftRange = make_pair(cell.first, l);
                Cell rightRange = make_pair(l + 1, cell.second);
                Message leftMessage = receivedMessages[leftRange];
                Message rightMessage = receivedMessages[rightRange];

                for (LhsStruct nonTerminal1 : leftMessage.NonTerminals) {
                    for (LhsStruct nonTerminal2 : rightMessage.NonTerminals) {

                        set<LhsStruct> LHS = g.getLHS(nonTerminal1.lhs, nonTerminal2.lhs);

                        for (LhsStruct lhs : LHS) {
                            string newsym = lhs.lhs;
                            double newScore = lhs.score * nonTerminal1.score * nonTerminal2.score;
                            string newSubParse =
                                    "(" + newsym + " " + nonTerminal1.subtree + " " + nonTerminal2.subtree + ")";
                            LhsStruct newLhs;
                            newLhs.lhs = newsym;
                            newLhs.score = newScore;
                            newLhs.subtree = newSubParse;
                            set<LhsStruct>::iterator got = newNonTerminals.find(newLhs);
                            if (got == newNonTerminals.end()) {
                                newNonTerminals.insert(newLhs);
                            } else {
                                LhsStruct oldLhs = *got;
                                if (oldLhs.score < newLhs.score) {
                                    // Keeping best score.
                                    newNonTerminals.erase(oldLhs);
                                    newNonTerminals.insert(newLhs);
                                }
                            }
                        }
                    }
                }
            }
            messageToBeSent.setNonTerminalsAndParse(newNonTerminals);
            if (cell.first == 0 && cell.second == N - 1) {
                // This is our final cell (located the top right corner). If we are at this cell then we do not need to
                // send any more messages. We are checking if a parse exists or not and printing the most probable parse
                // if more than one parse exist.
                milliseconds endMs = duration_cast<milliseconds>(
                        system_clock::now().time_since_epoch()
                );
                bool foundParse = false;
                for (LhsStruct lhsStruct : newNonTerminals) {
                    if (lhsStruct.lhs.compare("ROOT") == 0) {
                        foundParse = true;
                        cout << lhsStruct.subtree << "\n";
                        cout << lhsStruct.score << "\n";
                    }
                }
                if (!foundParse) {
                    cout << "Woops!! This sentence sucks!!\n";
                }
                cout << (endMs.count() - startMs.count()) << "\n";
            }
        }

        // Sending messages to all cells which depends on data from this cell. All such cells are located vertically up
        // and horizationally right to current cell. Cell at top-right corner doesn't require to send data to any cell.
        for (int i = cell.second + 1; i < N; i++) {
            int to_processID = cell2process[make_pair(cell.first, i)];
            int tag = tagHash(cell.first, cell.second);
            sendMessage(messageToBeSent.toString(), to_processID, tag);
        }
        for (int i = cell.first - 1; i >= 0; i--) {
            int to_processID = cell2process[make_pair(i, cell.second)];
            int tag = tagHash(cell.first, cell.second);
            sendMessage(messageToBeSent.toString(), to_processID, tag);
        }
    }
    MPI_Finalize();

    return 0;
}

// This method is responsible for receiving message from given source process and tagged as tag. We are using tags
// while passing messages because we might have to access messages out of order.
string receiveMessage(int source, int tag) {
    MPI::Status status;
    MPI::COMM_WORLD.Probe(source, tag, status);
    int messageLength = status.Get_count(MPI::CHAR);
    char *buf = new char[messageLength];
    MPI::COMM_WORLD.Recv(buf, messageLength, MPI::CHAR, source, tag, status);
    string message(buf, messageLength);
    delete[] buf;
    return message;
}

// Sending message to destination process tagged with given tag.
void sendMessage(const string &message, int dest, int tag) {
    MPI::COMM_WORLD.Send(message.c_str(), message.length(), MPI::CHAR, dest, tag);
}

// This method is used to evaluate tag we are assining to our outgoing message. As tag can only be an integer, we are
// hashing cell indices into a single integer.
int tagHash(int i, int j) {
    return (int) ((unsigned short) i) << 16 | (unsigned) j;
}

// Fills the given vector by spltting text by given seperator.
void split(vector<string> &tokens, const string &text, char sep) {
    int start = 0, end = 0;

    while ((end = text.find(sep, start)) != string::npos) {
        tokens.push_back(text.substr(start, end - start));
        start = end + 1;
    }
    tokens.push_back(text.substr(start));
}

// Given file containing sentence and delmiter used, it loads the sentence.
void loadSentence(string fileName, vector<string> &terminals) {
    ifstream in(fileName.c_str());
    if (!in) {
        cerr << "cant load file\n";
        exit(-1);
    }
    string line;
    while (getline(in, line)) {
        split(terminals, line, ' ');
    }
}
