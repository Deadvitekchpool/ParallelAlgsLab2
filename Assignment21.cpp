#include <iostream>
#include <fstream>
#include <string>
#include "mpi.h"

std::string gen_random(const int len) { //function for string generation
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    
    return tmp_s;
}

void create_and_fill_file(char* fname, int fsize) { //function for creating the file
	std::ofstream outfile(fname);
	std::string generated_str = gen_random(fsize);
	outfile << generated_str << std::endl;
	outfile.close();
}

int main(int argc, char **argv)
{
	int rank, size;
	char fname[] = "file1.txt";
	int fsize = 1000;
	create_and_fill_file(fname, fsize); //create file with certain size
	MPI_Init(&argc, &argv);
	MPI_File fh;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	char buf[fsize / size]; //define buf as the size of the file divided by number of processes
	MPI_File_open(MPI_COMM_WORLD, fname, MPI_MODE_RDONLY, MPI_INFO_NULL, &fh); //open the file
	MPI_File_set_view(fh, rank * fsize / size, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL); //set the view of opened file
	//as an offset, i passed the rank multiplied by the buf size
	MPI_File_read_all(fh, buf, fsize / size, MPI_CHAR, MPI_STATUS_IGNORE); //read file by all processes
	printf("process %d, buf=%s\n", rank, buf); //print the rank of the process and chars that it has read
	MPI_File_close(&fh); //close the file through file handler
	MPI_Finalize();
}