#include <mpi.h>
#include <stdio.h>
#include <vector>
#include "port.h"

typedef pair<int, int> Cell;

int main(int argc, char **argv) {
    // Initialize the MPI environment
    const int FROM_ABOVE = 0;
    const int FROM_BELOW = 1;
    const int MERGE = 2;


    int N = 4;
    int NUM_PROCESS = 2;
    int myrank = 1;
    bool done = false;
    int d = 0;
    int j = 0;
    int i = 0;
    int counter = 0;

    while (!done) {
        printf("i,j : %d,%d ; cell : %d\n", i, j, counter % NUM_PROCESS);
        if (counter % NUM_PROCESS == myrank) {

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

    MPI_Init(NULL, NULL);
    // Find out rank, size
    int ID;
    MPI_Comm_rank(MPI_COMM_WORLD, &ID);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    //int blocksize = DIM / world_size;

    printf("initial: %d\n", ID);
    MPI_Finalize();

}

