#include <iostream>
#include <fstream> 
#include "mpi.h"
#define BUFSIZE 100

void create_and_fill_file(char* fname) {
	std::ofstream outfile(fname);
	outfile << "Try to read this MPI!" << std::endl;
	outfile.close();
}

void delete_file(bool answer, char* fname) {
	if (answer) {
		std::remove(fname);
	}
}

int main(int argc, char **argv)
{
	int bufsize, num, sum;
	bool answer;
	char fname[] = "file.txt";
	std::cout << "Delete file after reading?" << std::endl;
	std::cin >> answer;
	create_and_fill_file(fname);
	MPI_Init(&argc, &argv);
	MPI_Status status;
	MPI_File fh;
	char buf[BUFSIZE];
	MPI_File_open(MPI_COMM_WORLD, fname, MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
	MPI_File_set_view(fh, 0, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);
	sum = 0;
	do {
		MPI_File_read(fh, buf, BUFSIZE, MPI_CHAR, &status);
		MPI_Get_count(&status, MPI_CHAR, &num);
		printf("buf=%s\n", buf);
		sum += num;
	} while (num >= BUFSIZE);
	MPI_File_close(&fh);
	delete_file(answer, fname);
	
	printf("The number of read symbols: %d\n", sum);
	MPI_Finalize();
}