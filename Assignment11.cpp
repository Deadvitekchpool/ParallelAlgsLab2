#include <iostream>
#include "mpi.h"
int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv); //initialize mpi with input arguments
    int rank, size;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &size); //set the size of mpi communicator(the number of processes)
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //set the ranks for processes
    int message = 1; //message to send
    //MPI_Sendrecv function simply sends message to the next process and receiving rec_message from previous process
    MPI_Sendrecv_replace(&message, 1, MPI_INT, rank == size - 1 ? 0 : rank + 1, 0,
     rank == 0 ? size - 1 : rank - 1, 0, MPI_COMM_WORLD, &status);
    printf("Process %d received message %d from process %d\n", rank, message, status.MPI_SOURCE);
    MPI_Sendrecv_replace(&message, 1, MPI_INT, rank == 0 ? size - 1 : rank - 1, 0,
     rank == size - 1 ? 0 : rank + 1, 0, MPI_COMM_WORLD, &status);
    printf("Process %d received message %d from process %d\n", rank, message, status.MPI_SOURCE);

    MPI_Finalize(); //shut down mpi services
    return 0;
}