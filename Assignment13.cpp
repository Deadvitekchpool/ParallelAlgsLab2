#include <iostream>
#include <cstdlib>
#include "mpi.h"
int main(int argc, char **argv)
{
	int size, rank;
	int n = 500; //matrices size;
	MPI_Init(&argc, &argv); 
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//matrix initialization by each process
	int** a = new int*[n]; //generate 2 initial matrices with values from 1 to 1000 and 1 result matrix C full of zeros
    int** b = new int*[n];
	long** c = new long*[n];
	double start, finish;
	for(int i = 0; i < n; ++i) {
        a[i] = new int[n];
        b[i] = new int[n];
        c[i] = new long[n];
    }
	for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a[i][j] = rand() % 1000 + 1;
            b[i][j] = rand() % 1000 + 1;
			c[i][j] = 0;
        }
    }

	MPI_Barrier(MPI_COMM_WORLD); //barrier process synchronization
	start = MPI_Wtime(); // start timing for each process
	// matrix multiplication
	for (int k = 0; k < n; k++) {
		for (int i = 0; i < n; i++){
			for (int j = 0; j < n; j++) {
				c[i][j] = c[i][j] + a[i][k] * b[k][j];
			}
		}
	}
	finish = MPI_Wtime(); // end of timing
	// output the execution time of matrix multiplication at each process
	printf("Process number %d performed matrix multiplication with time %f\n", rank, finish - start);
	MPI_Finalize();
	return 0;
}