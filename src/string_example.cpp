#include <mpi.h>
#include <stdio.h>
#include <vector>
#include "port.h"
#include <utility>

using namespace std;

typedef pair<int, int> Cell;

void sendMessage(const string &bla, int dest, int tag);

string receiveMessage(int source, int tag);

int main(int argc, char **argv) {
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);
    int myrank;
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    int NUM_PROCESS;
    MPI_Comm_size(MPI_COMM_WORLD, &NUM_PROCESS);

    // Sender
    if (myrank == 0) {
        string bla = "blablaabc234abc12323";
        int dest = 1;
        int tag = 1;
        sendMessage(bla, dest, tag);
    }
    // Receiver
    if (myrank == 1) {
        int source = 0;
        int tag = 1;

        string msg = receiveMessage(source, tag);


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

void sendMessage(const string &bla, int dest, int tag) {
    MPI::COMM_WORLD.Send(bla.c_str(), bla.length(), MPI::CHAR, dest, tag);
}

