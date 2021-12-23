#include <iostream>
#include <fstream> 
#include "mpi.h"
#define BUFSIZE 100

void create_and_fill_file(char* fname) { //function for file creation
	std::ofstream outfile(fname);
	outfile << "Try to read this MPI!" << std::endl;
	outfile.close();
}

void delete_file(bool answer, char* fname) { //function for deleting the file
	if (answer) {
		std::remove(fname);
	}
}

int main(int argc, char **argv)
{
	int bufsize, num, sum;
	bool answer;
	char fname[] = "file.txt";
	std::cout << "Delete file after reading?" << std::endl; //ask if file supposed to be deleted
	std::cin >> answer; //get the answer 0 or 1
	create_and_fill_file(fname); //create file
	MPI_Init(&argc, &argv);
	MPI_Status status;
	MPI_File fh; //file handler
	char buf[BUFSIZE];
	MPI_File_open(MPI_COMM_WORLD, fname, MPI_MODE_RDONLY, MPI_INFO_NULL, &fh); //open created file
	MPI_File_set_view(fh, 0, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL); //set opened file view(set the offset and type of values to read)
	sum = 0; //variable for counting chars
	do {
		MPI_File_read(fh, buf, BUFSIZE, MPI_CHAR, &status); //put chars from file to buf
		MPI_Get_count(&status, MPI_CHAR, &num); //count how much chars have we read
		printf("buf=%s\n", buf); //print chars that we read
		sum += num; //sum counted number to global sum
	} while (num >= BUFSIZE);
	MPI_File_close(&fh); //close the file through handler
	delete_file(answer, fname); //delete this file if needed
	
	printf("The number of read symbols: %d\n", sum); //print the number of chars
	MPI_Finalize();
}