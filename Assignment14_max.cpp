#include <iostream>
#include "mpi.h"
#define n 1000

using namespace std;

int main(int argc, char** argv) {
    int val[n]; //initial vector
    int rank, size;
    int minval;
    struct { //structs for reduce function input
        float value;
        int   index;
        int rank;
    } in, out;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    for (int i = 0; i < n; i++) { //initial vector initializing
        val[i] = i + rank + 1;
    }
    
    in.value = val[0]; //local max value search
    in.index = 0;
    for (int i=1; i < n; i++)
        if (in.value < val[i]) {
            in.value = val[i];
            in.index = i;
            in.rank = rank;
        }
    
    in.index = rank * n + in.index; //max value index calculation
    MPI_Reduce(&in, &out, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        minval = out.value;
        printf("Maximum value %d \n", minval);
    }
    
    MPI_Finalize();
    return 0;
}