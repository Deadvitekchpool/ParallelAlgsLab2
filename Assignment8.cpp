#include <cstdlib>
#include <stdio.h>
#include <string>
#include <math.h>
#include "mpi.h"
#define MAX 1000000

using namespace std;

int count_pow(int x) { //function for dynamic calculation of 10^power = MAX 
    int i = 0;
    while (x != 1) {
        x = x / 10;
        i++;
    }
    return i;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv); //initialize mpi
    int rank, size;
    double start, finish;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &size); //set the size of mpi communicator(the number of processes)
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //set the ranks for processes
    int power = count_pow(MAX); //calculate the power of MAX
// Latency calculation
    start = MPI_Wtime(); //start the timer
    for (int i = 0; i < 10; i++) { //repeat all actions 10 times
        char empty = '\0'; //empty message

        if (rank == 0) { //first process
            MPI_Send(&empty, 0, MPI_CHAR, 1, 0, MPI_COMM_WORLD); //send empty message
            MPI_Recv(&empty, 0, MPI_CHAR, 1, 0, MPI_COMM_WORLD, &status); //receive it back
        }
        else { //second process
            MPI_Recv(&empty, 0, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status); //receive empty message
            MPI_Send(&empty, 0, MPI_CHAR, 0, 0, MPI_COMM_WORLD); //send it back
        }
    }
    finish = MPI_Wtime(); //stop the timer
    double s = (finish - start) / (2 * 10); //calculate latency by dividing time on N number of iterations multiplied by 2
    printf("Latency is: %f sec\n", s);
// Bandwidth calculation
    for (int i = 1; i <= MAX; i *= 10) { //loop for increasing the size of the message
        int message[i];
        int L = i * sizeof(int); //calculate L length of message

        start = MPI_Wtime(); //start the timer
        for (int k = 0; k < 10; k++) { //repeat all actions 10 times
            if (rank == 0) { //first process
                srand(i);
                for (int j = 0; j < i; j++) { //generate vector with required length of numbers from 1 to 1000
                    message[j] = rand() % 1000 + 1;
                }
                MPI_Send(&message[0], i, MPI_INT, 1, 0, MPI_COMM_WORLD); //send this vector to second process
                MPI_Recv(&message[0], i, MPI_INT, 1, 0, MPI_COMM_WORLD, &status); //receive it back
            }
            else { //second process
                int rec_mes[i];
                MPI_Recv(&rec_mes[0], i, MPI_INT, 0, 0, MPI_COMM_WORLD, &status); //receive vector
                MPI_Send(&rec_mes[0], i, MPI_INT, 0, 0, MPI_COMM_WORLD); //send it back
            }
        }
        finish = MPI_Wtime(); //stop the timer
        printf("Bandwidth of message with length %d is: %f Mb/s\n", i, 2 * 10 * L / (finish - start)); //calculate and print current Bandwidth
    }
    
    MPI_Finalize(); //shut down mpi services
    return 0;
}