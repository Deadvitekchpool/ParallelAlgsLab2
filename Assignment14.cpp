#include <iostream>
#include "mpi.h"
#define n 1000

void max(void *a, void *b, int *l, MPI_Datatype *type) { //function for max value search
	((int*)b)[0] = -1;
	for (int i = 0; i < *l; i++) {
		if (((int*)b)[0] < ((int*)a)[i]) {
			((int*)b)[0] = ((int*)a)[i];
		}
	}
}

int main(int argc, char **argv)
{
	int rank, size, i;
	MPI_Init(&argc, &argv);
	MPI_Op op; //variable for user's operation
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int a[n]; //initial vector
	int b[n]; //receive vector for reduce
	for (i = 0; i < n; i++) { //initial vector filling
		a[i] = i + rank + 1;
	}
	printf("process %d a[n - 1] = %d\n", rank, a[n - 1]); //print process rank and last(max) value of the initial vector
 	MPI_Op_create(&max, 1, &op); //create user's operation
	MPI_Reduce(&a, &b, n, MPI_INT, op, 0, MPI_COMM_WORLD); //to apply this operation, reduce function is used
	if (rank == 0) { //print result
		// for (int i = 0; i < n; i++) {
		printf("b[n - 1] = %d\n", b[n - 1]);
		// }
	}
	MPI_Op_free(&op); //free memory of user's operation
	MPI_Finalize();
	return 0;
}