#include<mpi.h>
#include<cstdio>
#include<cmath>

double func(double x);

constexpr double PRECISE_PI = std::atan(1) * 4.0;
constexpr unsigned int CHUNK_NUMB = 100000000;
int main(int argc, char*argv[])
{
	int ps_rank = 0;
	int ps_numb = 0;
	double x_start = 0.0;
	double x_end = 1.0;
	double x_step = (x_end - x_start) / CHUNK_NUMB;
	double loc_res = 0.0;
	double pi = 0.0;
	double start_time = 0.0;
	double end_time = 0.0;
	double loc_time = 0.0;
	double time = 0.0;
	double curr_x = 0.0;

	MPI_Init(&argc, &argv);	

		MPI_Comm_rank(MPI_COMM_WORLD, &ps_rank);
		MPI_Comm_size(MPI_COMM_WORLD, &ps_numb);
		
		start_time = MPI_Wtime();
		for (unsigned int i = ps_rank; i < CHUNK_NUMB; i += ps_numb)
		{
			curr_x = x_start + (i + 0.5) * x_step;
			loc_res += func(curr_x);
		}
		loc_res *= x_step;
		MPI_Reduce(&loc_res,
			   &pi,
			   1,
			   MPI_DOUBLE,
			   MPI_SUM,
			   0,
			   MPI_COMM_WORLD);
		end_time = MPI_Wtime();
		loc_time = end_time - start_time;
		
		MPI_Reduce(&loc_time,
			   &time,
			   1,
			   MPI_DOUBLE,
			   MPI_MAX,
			   0,
			   MPI_COMM_WORLD);

		if (ps_rank == 0)
		{
			printf("Integration result: %12.11e\n", pi);
			printf("Numerical error: %12.11e\n", std::abs(pi - PRECISE_PI));
			printf("Computational time: %12.11e s\n", time);
		}

	MPI_Finalize();
}

double func(double x)
{
	return 4.0 / (1.0 + x * x);
}
