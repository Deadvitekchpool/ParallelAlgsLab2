#include <stdio.h>
#include <cstdlib>
#include "mpi.h"

bool is_odd(int x) {
	if (x % 2 == 0) {
		return false;
	}
	else {
		return true;
	}
}

int main(int argc, char* argv[]) {
	MPI_Init(&argc, &argv); //initialize mpi with input arguments
	int rank, n, i;
	int message[11];
	double start, finish;
	char *name = new char;
	int len;
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD, &n); //set the size of mpi communicator(the number of processes)
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); //set the ranks for processes
	MPI_Get_processor_name(name, &len);
	start = MPI_Wtime(); //save start time of the programm
	if (rank == 0) //code section for the process number 0(often the main process)
	{
		for (i = 1; i < n; i++) { // for loop for iteration through received messages
			long long mul = 1; //variable for calculating multiplication
			MPI_Recv(&message[0], 11, MPI_INT, MPI_ANY_SOURCE,
			         MPI_ANY_TAG, MPI_COMM_WORLD, &status); //receive message with 11 int values from any free process
			for (int j = 1; j < 11; j++) { //iteration through the received message array
				if (is_odd(message[j])) { //check if value is odd
					mul *= message[j]; //if odd, multiply the mul variable
				}
			}
			printf("The multiplication of odd numbers from process %d: %lld\n", message[0], mul); //print the number of the process and the result of 
		} //it's odd numbers multiplication
	}
	else { //code section for other processes
		srand(rank); //set the seed for rand function
		int a[11]; //initialize the array of numbers
		a[0] = rank; //first element is the rank of the process
		printf("Process number %d array:\n", rank); //print this rank
		for (i = 1; i < 11; i++) { //generate 10 random numbers from 1 to 1000 and print them
			a[i] = rand() % 1000 + 1;
			printf("a[%d] = %d\n", i, a[i]);
		}
		MPI_Send(&a[0], 11, MPI_INT, 0, 0, MPI_COMM_WORLD); //send generated array from current process to main process 0
	}
	finish = MPI_Wtime(); //save finish time of the programm
	printf("Execution time of the %s programm: %f\n", name, (finish - start) / n); //print the name of the processor and it's execution time of this programm
	MPI_Finalize(); //shut down mpi services
	return 0;
}