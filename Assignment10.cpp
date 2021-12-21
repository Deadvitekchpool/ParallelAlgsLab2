#include <iostream>
#include "mpi.h"
int main(int argc, char **argv)
{
	int rank, size, prev, next;
	int buf[2]; //buffer for incoming messages
	MPI_Init(&argc, &argv); //initialize mpi with input arguments
	MPI_Request reqs[4];
	MPI_Status stats[4]; //array of statuses for messages
	MPI_Comm_size(MPI_COMM_WORLD, &size); //set the size of mpi communicator
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); //set the ranks for processes
	prev = rank - 1; //previous process
	next = rank + 1; //next process
	if (rank == 0) prev = size - 1; //if rank of the process is 0, so his previous process is the last process
	if (rank == size - 1) next = 0; //if rank of the process is last, so his next process is the first process
	MPI_Irecv(&buf[0], 1, MPI_INT, prev, 5, MPI_COMM_WORLD, &reqs[0]); //receive message from previous process and put it to buf
	MPI_Irecv(&buf[1], 1, MPI_INT, next, 6, MPI_COMM_WORLD, &reqs[1]); //receive message from next process and put it to buf
	MPI_Isend(&rank, 1, MPI_INT, prev, 6, MPI_COMM_WORLD, &reqs[2]); //send the rank of current process to previous and next processes
	MPI_Isend(&rank, 1, MPI_INT, next, 5, MPI_COMM_WORLD, &reqs[3]);
	MPI_Waitall(4, reqs, stats); //wait for all processes to finish their actions
	
	//Your code here.
	//Here you need to display the number of the current process, and what it receives from the previous and next processes.
	printf("Current process number: %d\n", rank); //print current process number
	for (int i = 0; i < 2; i++) {
		printf("buf[%d] = %d\n", i, buf[i]); //print buf values
	}
	
	MPI_Finalize();
	return 0;
}