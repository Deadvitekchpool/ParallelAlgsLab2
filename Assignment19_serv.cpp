#include <stdio.h>
#include "mpi.h"
int main(int argc, char **argv)
{
	int r; //message to receive
	int msg = 200; //message to send
	MPI_Init(&argc, &argv);
	char port_name[MPI_MAX_PORT_NAME] = "1337"; //port name
	MPI_Status status;
	MPI_Comm intercomm;
	MPI_Open_port(MPI_INFO_NULL, port_name); //open port with given name
	printf("portname: %s\n", port_name);
	printf("Waiting for the client ...\n");
	MPI_Comm_accept(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm); //accept connection inside communicator
	printf("Client connected\n");
	MPI_Send(&msg, 1, MPI_INT, 0, 0, intercomm); //send message to client
	printf("Server sent value %d\n", msg);
	MPI_Recv(&r, 1, MPI_INT, 0, 0, intercomm, &status); //receive message from client
	MPI_Comm_free(&intercomm); //free communicator
	MPI_Close_port(port_name); //close the port
	printf("Server got value %d\n", r);
	MPI_Finalize();
	return 0;
}