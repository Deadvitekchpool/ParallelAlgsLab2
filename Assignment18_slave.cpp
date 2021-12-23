#include "mpi.h"
#include <iostream>
int main(int argc, char **argv)
{
	int rank, size;
	MPI_Comm intercomm;
	MPI_Init(&argc, &argv);
	MPI_Comm_get_parent(&intercomm); //get parent communicator from which whis script was launched
	MPI_Comm_size(intercomm, &size); //get the size
	MPI_Comm_rank(intercomm, &rank); //get the ranks
	if (rank != 2) { //processes 0 and 1 are sending their ranks
		MPI_Send(&rank, 1, MPI_INT, 0, rank, intercomm);	
	}
	printf("[Slave]: Slave process number %d is running\n", rank);
	
	if (rank == size - 1) { //process 2 is sending the size of communicator
		MPI_Send(&size, 1, MPI_INT, 0, rank, intercomm);
	}
	MPI_Finalize();
	return 0;
}