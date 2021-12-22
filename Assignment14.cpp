#include <stdio.h>
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
	int a[n];
	int b[] = {-1};
	int m[] = {-1};
	MPI_Init(&argc, &argv);
	MPI_Op op;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	for (i = 0; i < n; i++) a[i] = i + rank + 1;
	printf("process %d a[n - 1] = %d\n", rank, a[n - 1]);
	MPI_Op_create(&max, 1, &op);
	MPI_Reduce(&a, &b, n, MPI_INT, op, 0, MPI_COMM_WORLD);
	// MPI_Bcast(&b[0], 1, MPI_INT, 0, MPI_COMM_WORLD );
	printf("Process number %d calculated max: %d\n", rank, b[0]);
	for (int i = 0; i < n; i++) {
		if (m[0] < a[i]) {
			m[0] = a[i];
		}
	}
	MPI_Reduce(&a, &m, n, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
	// MPI_Bcast(&m[0], 1, MPI_INT, 0, MPI_COMM_WORLD );
	printf("Process number %d calculated max using mpi max: %d\n", rank, m[0]);
	MPI_Op_free(&op);
	// if (rank == 0) {
		// printf("b = %d\n", b[0]);
		// printf("m = %d\n", m[0]);
	// }
	MPI_Finalize();
	return 0;
}