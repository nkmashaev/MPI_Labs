#include <mpi.h>
#include <cstdio>

int main(int argc, char*argv[])
{
	int ps_rank = 0;
	int ps_numb = 1;
	double start_time = 0.0;
	double end_time = 0.0;
	double loc_time = 0.0;
	double time = 0.0;

	MPI_Init(&argc, &argv);
		start_time = MPI_Wtime();

		MPI_Comm_rank(MPI_COMM_WORLD, &ps_rank);
	
		if (ps_rank == 0)
		{
			MPI_Comm_size(MPI_COMM_WORLD, &ps_numb);
			printf("Number of processes: %d\n", ps_numb);
		}
		MPI_Barrier(MPI_COMM_WORLD);
		MPI_Bcast(&ps_numb, 1, MPI_INT, 0, MPI_COMM_WORLD);
		printf("Hello world from %d of %d processes\n", ps_rank, ps_numb);

		end_time = MPI_Wtime();
		loc_time = end_time - start_time;
		MPI_Reduce(&loc_time,
			   &time,
			   1,
			   MPI_DOUBLE,
			   MPI_MAX,
			   0,
			   MPI_COMM_WORLD);
			
	MPI_Finalize();
	
	if (ps_rank == 0)
	{
		printf("Computational time: %12.11e s\n", time);
	}
	return 0;
}
