#include "mpi.h"
#include <iostream>
int main(int argc, char **argv)
{
	int rank, size;
	MPI_Comm intercomm;
	MPI_Init(&argc, &argv);
	MPI_Comm_get_parent(&intercomm);
	MPI_Comm_size(intercomm, &size);
	MPI_Comm_rank(intercomm, &rank);
	if (rank != 0) {
		printf("[Slave]: Slave process number %d is running\n", rank);
	}
	MPI_Send(&rank, 1, MPI_INT, 0, rank, intercomm);
	if (rank == size - 1) {
		MPI_Send(&size, 1, MPI_INT, 0, rank, intercomm);
	}
	MPI_Finalize();
	return 0;
}