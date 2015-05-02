// Author: Wes Kendall
// Copyright 2011 www.mpitutorial.com
// This code is provided freely with the tutorials on mpitutorial.com. Feel
// free to modify it for your own use. Any distribution of the code must
// either provide a link to www.mpitutorial.com or keep this header in tact.
//
// MPI_Send, MPI_Recv example. Communicates the number -1 from process 0
// to processe 1.
//
#include <mpi.h>
#include <stdio.h>
#include <string.h> // memcpy

#define ITERATIONS 64
#define GRID_WIDTH  256
#define DIM  16     // assume a square grid

void display(int arr[]);

void compueteSameRowMessage(int grid[], int row, int message[]);

void displayFullGrid(int grid[]);

void displayGridRow(int grid[], int row);

void computeMessage(int grid[], int row, int message[]);

int main(int argc, char **argv) {
    // Initialize the MPI environment
    const int FROM_ABOVE = 0;
    const int FROM_BELOW = 1;
    const int MERGE = 2;
    /*int global_grid[DIM * DIM] = {0, 0, 0, 0,
                                  0, 0, 0, 0,
                                  1, 1, 1, 0,
                                  1, 0, 0, 0};*/

    /*int global_grid[N * N] = {0, 0, 0, 0, 0, 0,
                       0, 0, 0, 0, 0, 0,
                       0, 0, 0, 0, 0, 0,
                       0, 0, 0, 0, 0, 0,
                       1, 1, 0, 0, 0, 0,
                       1, 0, 0, 0, 0, 0};*/

    /*int global_grid[DIM * DIM] = {0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0,
                                  1, 1, 0, 0, 0, 0, 0, 0,
                                  1, 0, 0, 0, 0, 0, 0, 0};*/
    int global_grid[DIM * DIM] = {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    MPI_Init(NULL, NULL);
    // Find out rank, size
    int ID;
    int iters;
    MPI_Comm_rank(MPI_COMM_WORLD, &ID);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int upper_rank = (ID == 0) ? world_size - 1 : ID - 1;
    int lower_rank = (ID == world_size - 1) ? 0 : ID + 1;
    int blocksize = DIM / world_size;
    int startrow = ID * blocksize;
    int endrow = startrow + blocksize;

    if (ID == 0) {
        printf("initial:\n");
        displayFullGrid(global_grid);
    }

    for (int iters = 0; iters < ITERATIONS; iters++) {

        //if (ID == 0) {
        //printf("rank %d, iters %d initial:\n", ID, iters);
        //displayFullGrid(global_grid);
        //}

        for (int i = startrow; i < endrow; i++) {
            if (i == startrow) {
                int message[DIM];
                computeMessage(global_grid, i, message);
                //MPI_Send(&message, 1, column, upper_rank, FROM_BELOW, MPI_COMM_WORLD);
                MPI_Send(&message, DIM, MPI_INT, upper_rank, FROM_BELOW, MPI_COMM_WORLD);
                //printf("rank %d, row %d has sent a message to   %d (from below):", ID, i, upper_rank);
                //display(message);
            }
            if (i == endrow - 1) {

                int message[DIM];
                computeMessage(global_grid, i, message);
                //MPI_Send(&message, 1, column, lower_rank, FROM_ABOVE, MPI_COMM_WORLD);
                MPI_Send(&message, DIM, MPI_INT, lower_rank, FROM_ABOVE, MPI_COMM_WORLD);
                //printf("rank %d, row %d has sent a message to   %d (from above):", ID, i, lower_rank);
                //display(message);

            }
            if (i != endrow - 1 && i != startrow) {
                //printf("rank %d : row %d is  internal \n", ID, i);
            }
        }
        int aliveNeighbors[DIM * DIM] = {0};

        for (int i = startrow; i < endrow; i++) {
            int mFromAbove[DIM] = {0};
            int mFromBelow[DIM] = {0};
            int mFromSameRow[DIM] = {0};

            compueteSameRowMessage(global_grid, i, mFromSameRow);
            if (i == startrow) {
                //printf("rank %d: waiting for recv (from above) %d...", ID, i);
                MPI_Recv(mFromAbove, DIM, MPI_INT, upper_rank, FROM_ABOVE, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                //printf("rank %d: received a message from rank %d (from above):\n", ID, upper_rank);
                //display(mFromAbove);
            } else {
                //printf("rank %d, row %d: local message from above:", ID, i);
                computeMessage(global_grid, i - 1, mFromAbove);
                //display(mFromAbove);
                //printf("state of grid\n");
                //displayFullGrid(global_grid);
            }
            if (i == endrow - 1) {
                //printf("rank %d: waiting for recv (from below) %d...", ID, i);
                MPI_Recv(mFromBelow, DIM, MPI_INT, lower_rank, FROM_BELOW, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                //printf("rank %d: received a message from  rank %d (from below)\n:", ID, lower_rank);
                //display(mFromBelow);
            } else {
                //printf("rank %d, row%d: local message from below:", ID, i);
                computeMessage(global_grid, i + 1, mFromBelow);
                //display(mFromBelow);
            }

            if (i != endrow - 1 && i != startrow) {
                //printf("rank %d : row %d is internal\n", ID, i);
                computeMessage(global_grid, i + 1, mFromBelow);
                computeMessage(global_grid, i - 1, mFromAbove);

            }
            //all messages received
            for (int j = 0; j < DIM; j++) {
                aliveNeighbors[i * DIM + j] = mFromAbove[j] + mFromBelow[j] + mFromSameRow[j];
            }

            /*("\n********************************\nrank %d: Row %d has the following messages:\n", ID, i);
            printf("From Above  :");
            display(mFromAbove);
            printf("From SameRow:");
            display(mFromSameRow);
            printf("From Below  :");
            display(mFromBelow);
            printf("\n********************************\n");*/

        }

        //printf("rank %d Alive Neighbors\n", ID);
        //displayFullGrid(aliveNeighbors);

        for (int c = 0; c < DIM * DIM; c++) {
            int a = aliveNeighbors[c];
            if (a < 2) {
                global_grid[c] = 0;
            } else if ((a == 2 || a == 3) && (global_grid[c] == 1)) {
                //do nothing
            } else if (a == 3 && global_grid[c] == 0) {
                global_grid[c] = 1;
            } else if (a > 3) {
                global_grid[c] = 0;
            }
        }

        MPI_Barrier(MPI_COMM_WORLD);
        if (ID == 0 && iters == ITERATIONS - 1) { // && iters == ITERATIONS - 1
            for (int other_rank = 1; other_rank < world_size; other_rank++) {
                int gridCopy[DIM * DIM];
                MPI_Recv(gridCopy, DIM * DIM, MPI_INT, other_rank, MERGE, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                int startrow_otherrank = other_rank * blocksize;
                int endrow_otherrank = startrow_otherrank + blocksize;
                for (int i = startrow_otherrank; i < endrow_otherrank; i++) {
                    for (int j = 0; j < DIM; j++) {
                        global_grid[i * DIM + j] = gridCopy[i * DIM + j];
                    }
                }
            }

            printf("\nIteration %d: final grid:\n", iters);
            for (int j = 0; j < DIM * DIM; j++) {
                if (j % DIM == 0) {
                    printf("\n");
                }
                printf("%d  ", global_grid[j]);
            }
            printf("\n");


        } else if (ID > 0 && iters == ITERATIONS - 1) {// && iters == ITERATIONS - 1
            MPI_Send(&global_grid, DIM * DIM, MPI_INT, 0, MERGE, MPI_COMM_WORLD);
        } else {
            //do nothing
        }
    }

    MPI_Finalize();

}

void displayFullGrid(int grid[]) {
    for (int i = 0; i < DIM; i++) {
        displayGridRow(grid, i);
    }
}

void displayGridRow(int grid[], int row) {
    printf("[");
    for (int j = 0; j < DIM; j++) {
        printf("%d,", grid[row * DIM + j]);
    }
    printf("]\n");
}

void compueteSameRowMessage(int grid[], int row, int message[]) {
    for (int j = 0; j < DIM; j++) {
        int ad_left, ad_right;
        if (j == 0) {
            ad_left = grid[row * DIM + (DIM - 1)];
        } else {
            ad_left = grid[row * DIM + (j - 1)];
        }

        if (j == DIM - 1) {
            ad_right = grid[row * DIM + 0];
        } else {
            ad_right = grid[row * DIM + (j + 1)];
        }
        message[j] = ad_left + ad_right;
    };
}

void computeMessage(int grid[], int row, int message[]) {
    for (int j = 0; j < DIM; j++) {
        int ad_left, ad, ad_right;
        ad = grid[row * DIM + j];

        if (j == 0) {
            ad_left = grid[row * DIM + (DIM - 1)];
        } else {
            ad_left = grid[row * DIM + (j - 1)];
        }

        if (j == DIM - 1) {
            ad_right = grid[row * DIM + 0];
        } else {
            ad_right = grid[row * DIM + (j + 1)];
        }
        message[j] = ad_left + ad + ad_right;
    };
}

void display(int arr[]) {
    printf("[");
    for (int i = 0; i < DIM; i++) {
        printf("%d,", arr[i]);
    }
    printf("]\n");
}
