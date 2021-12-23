#include <stdio.h>
#include <string.h>
#include "mpi.h"
int main(int argc, char **argv)
{
	int rank;
	int msg = 420;
	int rec_msg;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	char port_name[MPI_MAX_PORT_NAME];
	MPI_Comm intercomm;
	strcpy(port_name, argv[1]);
	printf("Attempt to connect ...\n");
	MPI_Comm_connect(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm);
	printf("Server connection\n");
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Send(&msg, 1, MPI_INT, 0, 0, intercomm);
	printf("Client sent value %d\n", msg);
	MPI_Recv(&rec_msg, 1, MPI_INT, 0, 0, intercomm, &status);
	printf("Client got value %d\n", rec_msg);
	MPI_Finalize();
	return 0;
}