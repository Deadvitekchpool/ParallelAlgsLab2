#include <iostream>
#include "mpi.h"

using namespace std;

int main(int argc, char* argv[]) {
	MPI_Init(&argc, &argv); //initialize MPI with inputed arguments
	int rank, n, i, message;
	MPI_Status status;
	MPI_Comm_size(MPI_COMM_WORLD, &n); //set the size of mpi communicator(the number of processes)
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); //set the ranks for processes
	if (rank == 0) //code section for the process number 0(often the main process)
	{
		cout << "Hello from process " << rank << "\n"; //in this process we are receiving messages from all other processes and output the numbers
		for (i  = 1; i < n; i++) { // of these processes to the console
			MPI_Recv(&message, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status); //function for receiving messages from other processes
			cout << "Hello from process " << message << endl;
		}
	}
	else MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); //code section for other processes where we are sending messages to the main process
	MPI_Finalize(); //shut down mpi services
	return 0;
}