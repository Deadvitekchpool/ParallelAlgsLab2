#include <stdio.h>
#include <string.h>
#include "mpi.h"
int main(int argc, char **argv)
{
	int rank;
	int msg = 100; //message to send
	int rec_msg; //message to receive
	MPI_Status status;
	MPI_Init(&argc, &argv);
	char port_name[MPI_MAX_PORT_NAME]; //array for storing port
	MPI_Comm intercomm;
	strcpy(port_name, argv[1]); //read port name from console arguments and copy it to port_name array
	printf("Attempt to connect ...\n");
	MPI_Comm_connect(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm); //connect to inputed port
	printf("Server connection\n");
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); //get the ranks
	MPI_Send(&msg, 1, MPI_INT, 0, 0, intercomm); //send message to server
	printf("Client sent value %d\n", msg); //print it
	MPI_Recv(&rec_msg, 1, MPI_INT, 0, 0, intercomm, &status); //receive message from server
	printf("Client got value %d\n", rec_msg); //print it
	MPI_Finalize();
	return 0;
}