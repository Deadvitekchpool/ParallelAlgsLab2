#include <iostream>
#include <mpi.h>
using namespace std;
int main(int argc, char **argv)
{
	int rank, size, ibuf;
	MPI_Status status;
	float rbuf;
	MPI_Init(&argc, &argv); //initialize mpi with input arguments
	MPI_Comm_size(MPI_COMM_WORLD, &size); //set the size of mpi communicator(the number of processes)
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); //set the ranks for processes
	ibuf = rank; //first buffer is the int number of process
	if (rank == 1) {
		MPI_Send(&ibuf, 1, MPI_INT, 0, 1, MPI_COMM_WORLD); //from first process send it's int number with tag 1
		}
	if (rank == 2) {
		rbuf = 1.0 * rank; //make buffer float
		MPI_Send(&rbuf, 1, MPI_FLOAT, 0, 2, MPI_COMM_WORLD); //from second process send it's float number with tag 2
		}
	if (rank == 0) { //main process code section
		for (int i = 1; i < 3; i++) { //added loop to receive both messages
			MPI_Probe(MPI_ANY_SOURCE, i, MPI_COMM_WORLD, &status); //try to receive message(status structure updates after this action)
			if (status.MPI_TAG == 1) { //if message tag is 1, that means that message came from the first process and we have to receive int value
				MPI_Recv(&ibuf, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, &status); //receive message from the first process
				cout << "Process 0 recv " << ibuf << " from process 1\n"; //print it
			}
			else if (status.MPI_TAG == 2) { //if message tag is 2, that means that message came from the second process and we have to receive float value
				MPI_Recv(&rbuf, 1, MPI_FLOAT, 2, 2, MPI_COMM_WORLD, &status); //receive message from the second process
				cout << "Process 0 recv " << rbuf << " from process 2\n";// print it
			}
		}
	}
	MPI_Finalize();
}