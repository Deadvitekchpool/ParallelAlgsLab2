#include "mpi.h"
#include <iostream>
int main(int argc, char **argv)
{
	int rank, size, i, rbuf;
	MPI_Init(&argc, &argv);
	MPI_Group group, new_group; //two different groups of processes
	MPI_Comm new_comm;
	int ranks[128], new_rank;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_group(MPI_COMM_WORLD, &group);

	for (i = 0; i < size / 2; i++) { //initialize ranks array
		ranks[i] = i;
	}

	if (rank < size / 2) { //divide all processes into 2 groups by it's rank
		MPI_Group_incl(group, size / 2, ranks, &new_group); //stay in initial group
	}
	else {
		MPI_Group_excl(group, size / 2, ranks, &new_group); //exclude process to another group
	}
	
	MPI_Comm_create(MPI_COMM_WORLD, new_group, &new_comm); //create new communicator
	MPI_Allreduce(&rank, &rbuf, 1, MPI_INT, MPI_SUM, new_comm); //sum processes ranks from both groups
	MPI_Group_rank(new_group, &new_rank); //make new ranks in new group
	
	//Display values: "rank =, newrank =, rbuf ="
	printf("rank = %d, newrank = %d, rbuf = %d\n", rank, new_rank, rbuf);
	
	MPI_Finalize();
	return 0;
}