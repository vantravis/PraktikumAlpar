
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char** argv){

	int rank, banyak_processor, hitung_recv;
	MPI_Request minta;
	MPI_Status status;
	double sBuf[100], rBuf[100];

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &banyak_processor);

	if (rank == 0) {

		MPI_Isend(sBuf, 100, MPI_DOUBLE, 1, 10, MPI_COMM_WORLD, &minta);
		MPI_Recv(rBuf, 100, MPI_DOUBLE, 1, 22, MPI_COMM_WORLD, &status);
		MPI_Wait(&minta, &status);

	}
	else if(rank==1){
		
		MPI_Isend(sBuf, 100, MPI_DOUBLE, 0, 10, MPI_COMM_WORLD, &minta);
		MPI_Recv(rBuf, 100, MPI_DOUBLE, 0, 22, MPI_COMM_WORLD, &status);
		MPI_Wait(&minta, &status);

	}

	MPI_Get_count(&status, MPI_DOUBLE, &hitung_recv);
	printf(" Processor: %d | Sumber: %d | Tag: %d | Total Terima: %d\n ", rank, status.MPI_SOURCE, status.MPI_TAG, hitung_recv);	
	MPI_Finalize();
}

