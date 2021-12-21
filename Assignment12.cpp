#include <iostream>
#include "mpi.h"
int main(int argc, char **argv)
{
	MPI_Init(&argc, &argv); //initialize mpi
	int rank, size;
	MPI_Request reqs[4]; //requests array
	MPI_Status stats[4]; //statuses array
	MPI_Comm_size(MPI_COMM_WORLD, &size); //set the number of processes
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); //set the ranks of the processes
	int rbuf[2]; //receive buffer
	int sbuf[2]; //send buffer
	int prev = rank - 1; //previous process rank
	int next = rank + 1; //next process rank
	if (rank == 0) prev = size - 1;
	if (rank == size - 1) next = 0;
	MPI_Recv_init(&rbuf[0], 1, MPI_INT, prev, 5, MPI_COMM_WORLD, &reqs[0]); //create a persistent receive request from previous process
	MPI_Recv_init(&rbuf[1], 1, MPI_INT, next, 6, MPI_COMM_WORLD, &reqs[1]); //create a persistent receive request from next process
	MPI_Send_init(&sbuf[0], 1, MPI_INT, prev, 6, MPI_COMM_WORLD, &reqs[2]); //create a persistent send request from previous process
	MPI_Send_init(&sbuf[1], 1, MPI_INT, next, 5, MPI_COMM_WORLD, &reqs[3]); //create a persistent send request from next process
	for (int i = 0; i < 2; i++) { //loop for output
		sbuf[0] = rank; //initialize send buffer with current process rank
		sbuf[1] = rank;
		MPI_Startall(4, reqs); //start all requests
		MPI_Waitall(4, reqs, stats); //wait for their completion
		printf("Process %d rbuf[%d] = %d\n", rank, i, rbuf[i]); //print received messages
	}
	MPI_Request_free(&reqs[0]); //free all persistent requests
	MPI_Request_free(&reqs[1]);
	MPI_Request_free(&reqs[2]);
	MPI_Request_free(&reqs[3]);

	MPI_Finalize(); //shut down mpi services
	return 0;
}