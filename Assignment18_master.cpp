#include "mpi.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char **argv)
{
	int size, rank, rank1, rank2;
	MPI_Status status;
	MPI_Comm intercomm;
	char slave[10] = "./slave.o";
	int ranks;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_spawn(slave, MPI_ARGV_NULL, 3, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm, MPI_ERRCODES_IGNORE);
	printf("Master process %d is running\n", rank);
	MPI_Recv(&rank1, 1, MPI_INT, 0, 0, intercomm, &status);
	MPI_Recv(&rank2, 1, MPI_INT, 1, 1, intercomm, &status);
	MPI_Recv(&ranks, 1, MPI_INT, 2, 2, intercomm, &status);

	printf("[Master]: Slave processes %d and %d are working\n", rank1, rank2);
	printf("[Master]: There are %d slave processes now", ranks);
	MPI_Finalize();
	return 0;
}