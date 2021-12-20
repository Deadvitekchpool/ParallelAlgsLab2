#include <stdio.h>
#include "mpi.h"
#include <algorithm>
#define HALF 250000 //define the size of vectors
#define FULL HALF * 2

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv); //initialize mpi
    int rank, size, i, j;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &size); //set the size of mpi communicator(the number of processes)
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //set the ranks for processes

    if (rank == 0) { //main process code section
        long long sum = 0; //output sum variable
        int a[FULL]; //first vector
        int b[FULL]; //second vector
        fill_n(a, FULL, 1); //filling vectors with 1
        fill_n(b, FULL, 1);
        
        for (i = 0; i < 2; i++) { //send half of each vector to each process 
            MPI_Send(&a[i * HALF], HALF, MPI_INT, i + 1, 0, MPI_COMM_WORLD);
            MPI_Send(&b[i * HALF], HALF, MPI_INT, i + 1, 0, MPI_COMM_WORLD);
        }

        for (i = 1; i <= 2; i++) { //receive both halfs of multiplied vectors
            int mul_elems[HALF];   
            MPI_Recv(&mul_elems[0], HALF, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            // MPI_Recv(&mul_elems[HALF], HALF, MPI_INT, 2, 0, MPI_COMM_WORLD, &status);

            for (j = 0; j < HALF; j++) { //summarize all elements in received arrays
                // printf("%lld\n", mul_elems[j]);
                sum += mul_elems[j];
            }
        }
        printf("Scalar multiplication of vectors A and B: %lld\n", sum);
    }
    else { //other processes code section
        int muls[HALF];
        int recv_a[HALF];
        int recv_b[HALF];

        MPI_Recv(&recv_a[0], HALF, MPI_INT, 0, 0, MPI_COMM_WORLD, &status); //receive half of the vector by each process
        MPI_Recv(&recv_b[0], HALF, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        printf("Process %d received it's parts of vectors\n", rank);

        for (i = 0; i < HALF; i++) { //multiply received vectors elements
            muls[i] = recv_a[i] * recv_b[i];
        }
        MPI_Send(&muls[0], HALF, MPI_INT, 0, 0, MPI_COMM_WORLD); //send the result array to main process
    }
    MPI_Finalize(); //shut down mpi services
    return 0;
}