#include <iostream>
#include "mpi.h"
#define n 1000

void smod5(void *a, void *b, int *l, MPI_Datatype *type) {
	int i;
	for (i = 0; i < *l; i++)
		((int*)b)[i] = (((int*)a)[i] + ((int*)b)[i]) % 5;
}

void max(void *a, void *b, int *l, MPI_Datatype *type) {
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
	MPI_Op op;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int a[n];
	int b[n];
	int m[] = {-1};
	for (i = 0; i < n; i++) a[i] = i + rank + 1;
	printf("process %d a[n - 1] = %d\n", rank, a[n - 1]);
	MPI_Op_create(&max, 1, &op);
	MPI_Reduce(&a, &b, n, MPI_INT, op, 0, MPI_COMM_WORLD);
	if (rank == 0) {
		// for (int i = 0; i < n; i++) {
		printf("b[n - 1] = %d\n", b[n - 1]);
		// }
	}
	MPI_Op_free(&op);
	MPI_Finalize();
	return 0;
}