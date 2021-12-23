#include "mpi.h"
#include <iostream>
int main(int argc, char **argv)
{
	int size, rank, position, i;
	float a[10];
	char b[10], buf[100];
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	for (i = 0; i < 10; i++) {
		a[i] = rank + 1.0;
		if (rank == 0) b[i] = 'a';
		else b[i] = 'b';
	}
	position = 0;
	printf("Process number %d\n", rank);
	printf("--------Before packing and broadcasting--------\n");
	for (int i = 0; i < 10; i++) {
		printf("a[%d] = %f\n", i, a[i]);
		printf("b[%d] = %d\n", i, b[i]);
	}
	if (rank == 0) { //root process code section
		MPI_Pack(a, 10, MPI_FLOAT, buf, 100, &position, MPI_COMM_WORLD); //pack vectors a and b together for broadcasting from root process
		MPI_Pack(b, 10, MPI_CHAR, buf, 100, &position, MPI_COMM_WORLD);
		MPI_Bcast(buf, 100, MPI_PACKED, 0, MPI_COMM_WORLD); //broadcast them to every other process
	}
	else {
		MPI_Bcast(buf, 100, MPI_PACKED, 0, MPI_COMM_WORLD); //same broadcast
		position = 0;
		MPI_Unpack(buf, 100, &position, a, 10, MPI_FLOAT, MPI_COMM_WORLD); //unpack a and b after broadcasting
		MPI_Unpack(buf, 100, &position, b, 10, MPI_CHAR, MPI_COMM_WORLD);
	}
	printf("Process number %d\n", rank);
	printf("--------After packing and broadcasting--------\n");
	for (int i = 0; i < 10; i++) {
		printf("a[%d] = %f\n", i, a[i]);
		printf("b[%d] = %d\n", i, b[i]);
	}
	MPI_Finalize();
}