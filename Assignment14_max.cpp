#include <iostream>
#include "mpi.h"
#define n 1000

using namespace std;

int main(int argc, char** argv) {
    int val[n]; /* local array of values */
    int count; /* local number of values */
    int rank, size, minrank, minindex;
    int minval;
    struct {
        float value;
        int   index;
        int rank;
    } in, out;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    for (int i = 0; i < n; i++) {
        val[i] = i + rank + 1;
    }
    /* local minloc */
    in.value = val[0];
    in.index = 0;
    for (int i=1; i < n; i++)
        if (in.value < val[i]) {
            in.value = val[i];
            in.index = i;
            in.rank = rank;
        }
    /* global minloc */
    
    in.index = rank * n + in.index;
    MPI_Reduce(&in, &out, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
        /* At this point, the answer resides on process root
         */
    if (rank == 0) {
        /* read answer out
         */
        minval = out.value;
        minrank = out.rank;
        minindex = out.index;
        printf("Minimum value %d \n", minval);
    }
    
    MPI_Finalize();
    return 0;
}