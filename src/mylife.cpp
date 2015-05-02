#include <mpi.h>
#include <stdio.h>
#include <vector>
#include "port.h"
#include <utility>

using namespace std;

typedef pair<int, int> Cell;

int main(int argc, char **argv) {
    // Initialize the MPI environment
    const int FROM_ABOVE = 0;
    const int FROM_BELOW = 1;
    const int MERGE = 2;
    MPI_Init(NULL, NULL);
    int myrank;
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    int NUM_PROCESS;
    MPI_Comm_size(MPI_COMM_WORLD, &NUM_PROCESS);

    vector<Cell> processStack;

    int N = 4;
    bool done = false;
    int d = 0;
    int j = 0;
    int i = 0;
    int counter = 0;

    while (!done) {
        //printf("i,j : %d,%d ; cell : %d\n", i, j, counter % NUM_PROCESS);
        if (counter % NUM_PROCESS == myrank) {
            processStack.push_back(make_pair(i,j));
        }
        counter++;
        i++;
        j++;
        if (j == N) {
            i = 0;
            d++;
            j = d;
        }
        if (i == 0 && j == N) {
            done = true;
        }
    }

    printf("initial stack: %d\n", myrank);
    for (Cell cell : processStack){
        printf("%d, cell: %d,%d\n", myrank, cell.first, cell.second);
    }
    MPI_Finalize();

  return 0;
}

