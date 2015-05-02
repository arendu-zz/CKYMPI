#include <mpi.h>
#include <stdio.h>
#include <vector>
#include "port.h"
#include <utility>

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

int main(int argc, char **argv) {
    // Initialize the MPI environment


    MPI_Init(NULL, NULL);
    int myProcessID;
    MPI_Comm_rank(MPI_COMM_WORLD, &myProcessID);
    int NUM_PROCESS;
    MPI_Comm_size(MPI_COMM_WORLD, &NUM_PROCESS);
    INT2VEC process2stack;
    CELL2INT cell2process;


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
        if (cell.first == cell.second) {

        } else {
            vector<Cell> recievedCells;
            for (int j = cell.first; j < cell.second; j++) { // receive from previous row cells
                //receive form (cell.first, j)
                int recv[2] = {-1};
                int from_processID = cell2process[make_pair(cell.first, j)];
                int tag = tagHash(cell.first, j);
//                printf("process %d cell (%d,%d) trying from process %d cell (%d,%d), tag %d\n", myProcessID, cell.first,
//                       cell.second, from_processID, cell.first, j, tag);
                MPI_Recv(recv, 2, MPI_INT, from_processID, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                Cell reCell = make_pair(recv[0], recv[1]);
//                printf("process %d cell (%d,%d) received from process %d cell (%d,%d)\n", myProcessID, cell.first,
//                       cell.second, from_processID, recv[0], recv[1]);
                recievedCells.push_back(reCell);
            }
            for (int i = cell.second; i > cell.first; i--) {
                //reveice from (i, cell.second)
                int recv[2] = {-1};
                int from_processID = cell2process[make_pair(i, cell.second)];
                int tag = tagHash(i, cell.second);
//                printf("process %d cell (%d,%d) trying from process %d cell (%d,%d), tag %d\n", myProcessID, cell.first,
//                       cell.second, from_processID, i, cell.second, tag);
                MPI_Recv(recv, 2, MPI_INT, from_processID, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                Cell reCell = make_pair(recv[0], recv[1]);
//                printf("process %d cell (%d,%d) received from process %d cell (%d,%d)\n", myProcessID, cell.first,
//                       cell.second, from_processID, recv[0],
//                       recv[1]);
                recievedCells.push_back(reCell);
            }
            for (Cell reCell : recievedCells) {
                printf("process %d cell (%d,%d) , received  content (%d,%d)\n", myProcessID, cell.first, cell.second,
                       reCell.first, reCell.second);
            }
        }

        // now compute the sends
        if (cell.first == cell.second) {
            // no wait  need to start sending
            int message[2];
            message[0] = cell.first;
            message[1] = cell.second;
            for (int i = cell.second + 1; i < N; i++) {
                //send to (cell.first, i)
                int to_processID = cell2process[make_pair(cell.first, i)];
                int tag = tagHash(cell.first, cell.second);
//                printf("process %d sending (%d,%d)  to process %d (%d,%d), tag %d\n", myProcessID, cell.first,
//                       cell.second, to_processID, cell.first, i, tag);
                MPI_Send(&message, 2, MPI_INT, to_processID, tag, MPI_COMM_WORLD);
                //string msg;
                //msg = to_string(cell.first) + "," + to_string(i), " msgid:" + to_string(mid);
                //sendMessage(msg, to_processID, TAG);
            }
            for (int i = cell.first - 1; i >= 0; i--) {
                // send to (i, cell.second)
                int to_processID = cell2process[make_pair(i, cell.second)];
                int tag = tagHash(cell.first, cell.second);
//                printf("process %d sending (%d,%d)  to process %d (%d,%d), tag %d\n", myProcessID, cell.first,
//                       cell.second, to_processID, i, cell.second, tag);
                MPI_Send(&message, 2, MPI_INT, to_processID, tag, MPI_COMM_WORLD);
            }
        } else {
            int message[2];
            message[0] = cell.first;
            message[1] = cell.second;
            for (int i = cell.second + 1; i < N; i++) {
                //send to (cell.first, i)
                int to_processID = cell2process[make_pair(cell.first, i)];
                int tag = tagHash(cell.first, cell.second);
//                printf("process %d sending (%d,%d)  to process %d (%d,%d), tag %d\n", myProcessID, cell.first,
//                       cell.second, to_processID, cell.first, i, tag);
                MPI_Send(&message, 2, MPI_INT, to_processID, tag, MPI_COMM_WORLD);
                //string msg;
                //msg = to_string(cell.first) + "," + to_string(i), " msgid:" + to_string(mid);
                //sendMessage(msg, to_processID, TAG);
            }
            for (int i = cell.first - 1; i >= 0; i--) {
                // send to (i, cell.second)
                int to_processID = cell2process[make_pair(i, cell.second)];
                int tag = tagHash(cell.first, cell.second);
//                printf("process %d sending (%d,%d)  to process %d (%d,%d), tag %d\n", myProcessID, cell.first,
//                       cell.second, to_processID, i, cell.second, tag);
                MPI_Send(&message, 2, MPI_INT, to_processID, tag, MPI_COMM_WORLD);
            }
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
    return i + j; //(int) ((unsigned short) i) << 16 | (unsigned) j;
}

void sendMessage(const string &bla, int dest, int tag) {
    MPI::COMM_WORLD.Send(bla.c_str(), bla.length(), MPI::CHAR, dest, tag);
}

