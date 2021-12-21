#include <stdio.h>
#include <algorithm>
#include "mpi.h"
#define SIZE 1000000
#define HALF SIZE / 2

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv); //initialize mpi with input arguments
	int rank, size;
    double start, finish;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &size); //set the size of mpi communicator(the number of processes)
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //set the ranks for processes
    int a[SIZE]; //initial vector
    int b = 0; //total sum
    int buf; //variable for receiving messages
    fill_n(a, SIZE, 1); //fill the initial vector with 1

//cascade doubling section
    start = MPI_Wtime(); //start the timer
    for (int i = 0; i < 5; i++) { //divide the initial vector into 5 parts
        if (rank == 0) { //main process code section
            MPI_Send(&a[i * SIZE / 10], SIZE / 10, MPI_INT, 1, 0, MPI_COMM_WORLD); //send 1/10 of the initial array from the beginning to the first process
            MPI_Send(&a[SIZE - (i + 1) * SIZE / 10], SIZE / 10, MPI_INT, 2, 0, MPI_COMM_WORLD); //send 1/10 of the initial array from the end to the second process
            MPI_Recv(&buf, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status); //receive the sum from the first process
            b += buf; //sum received value with global sum
            MPI_Recv(&buf, 1, MPI_INT, 2, 0, MPI_COMM_WORLD, &status); //receive the sum from the second process
            b += buf; //sum received value with global sum
        }
        else { //other processes code section
            int message[SIZE / 10]; //array for received part of the initial vector
            int sum = 0; //sum variable
            MPI_Recv(&message[0], SIZE / 10, MPI_INT, 0, 0, MPI_COMM_WORLD, &status); //receive 1/10 of the initial vector
            for (int i = 0; i < SIZE / 10; i++) { //sum all received values
                sum += message[i];
            }
            MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); //send this sum to the main process
        }
    }
    finish = MPI_Wtime(); //stop timer
    printf("Total sum with cascade scheme: %d\n", b); //print total sum
    printf("Total sum time with cascade scheme: %f\n", finish - start); //print counted time
    b = 0;
//mpi reduce section
    start = MPI_Wtime(); //start the timer
    int local_sum = 0; //variable for local sum on each process
    for (int i = 0; i < SIZE / size; i++) { //calculate local sum of current process
        local_sum += a[i];
    }
    printf("Local sum for process %d: %d\n", rank, local_sum); //print this sum

    MPI_Reduce(&local_sum, &b, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD); //apply mpi reduce for all local sums into the total sum b
    finish = MPI_Wtime(); //stop the timer
    if (rank == 0) {
        printf("Sum algorithm using mpi reduce: %d\n", b); //print total sum from main process
        printf("Sum algorithm time using mpi reduce: %f\n", finish - start); //print counted time from main process
    }
    
    MPI_Finalize(); //shut down mpi services
    return 0;
}