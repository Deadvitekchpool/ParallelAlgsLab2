#include <stdio.h>
#include <algorithm>
#include "mpi.h"
#define SIZE 100
#define HALF SIZE / 2

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv); //initialize mpi with input arguments
	int rank, size;
    double start, finish;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &size); //set the size of mpi communicator(the number of processes)
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //set the ranks for processes
    int a[SIZE];
    int b = 0;
    fill_n(a, SIZE, 1);

    start = MPI_Wtime();
    for (int i = 0; i < SIZE; i++) {
        b += a[i];
    }
    finish = MPI_Wtime();
    printf("Straight forward sum: %d\n", b);
    printf("Straight forward sum algorithm time: %f\n", (finish - start) / (double)size);
    b = 0;

    start = MPI_Wtime();
    int local_sum = 0;
    for (int i = 0; i < SIZE / size; i++) {
        local_sum += a[i];
    }
    printf("Local sum for process %d: %d\n", rank, local_sum);

    // MPI_Send(&a[0], HALF, MPI_INT, 1, 0, MPI_COMM_WORLD);
    // MPI_Send(&a[HALF], HALF, MPI_INT, 2, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_sum, &b, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    finish = MPI_Wtime();
    if (rank == 0) {
        printf("Sum algorithm using mpi reduce: %d\n", b);
        printf("Sum algorithm time using mpi reduce: %f\n", (finish - start) / size);
    }
    // MPI_Recv(&message[0], HALF, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    // MPI_Send(&message[0], HALF, MPI_INT, 0, 0, MPI_COMM_WORLD);
    
    MPI_Finalize();
    return 0;
}