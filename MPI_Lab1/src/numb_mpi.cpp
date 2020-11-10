#include<mpi.h>
#include<cstdio>
#include<cmath>


bool is_pow_2(unsigned int val);
bool is_prime(unsigned int val);

constexpr unsigned int CHUNK_SIZE = 10000000;
int main(int argc, char*argv[])
{
	int ps_rank = 0;
	int ps_numb = 1;
	double start_time = 0.0;
	double end_time = 0.0;
	double loc_time = 0.0;
	double time = 0.0;

	unsigned int loc_prime_counter = 0;
	unsigned int loc_deg_2_counter = 0;
	unsigned int prime_counter = 0;
	unsigned int deg_2_counter = 0;

	MPI_Init(&argc, &argv);

		MPI_Comm_rank(MPI_COMM_WORLD, &ps_rank);
		MPI_Comm_size(MPI_COMM_WORLD, &ps_numb);
		
		start_time = MPI_Wtime();
		for (unsigned int i = ps_rank; i < CHUNK_SIZE; i += ps_numb)
		{
			if (is_prime(i))
			{
				loc_prime_counter += 1;
			}
			if (is_pow_2(i))
			{
				loc_deg_2_counter += 1;
			}
		}
		MPI_Reduce(&loc_prime_counter,
			   &prime_counter,
			   1,
			   MPI_UNSIGNED,
			   MPI_SUM,
			   0,
			   MPI_COMM_WORLD
			  );
		MPI_Reduce(&loc_deg_2_counter,
			   &deg_2_counter,
			   1,
			   MPI_UNSIGNED,
			   MPI_SUM,
			   0,
			   MPI_COMM_WORLD
			  );
		end_time = MPI_Wtime();
		loc_time = end_time - start_time;
		
		MPI_Reduce(&loc_time,
			   &time,
			   1,
			   MPI_DOUBLE,
			   MPI_MAX,
			   0,
			   MPI_COMM_WORLD
			  );

		if (ps_rank == 0)
		{
			printf("Size of prime number\'s amount: %d\n", prime_counter);
			printf("Number of degree of two: %d\n", deg_2_counter);
			printf("Time per clock: %12.11e s\n", MPI_Wtick());
			printf("Computational time: %12.11e s\n", time);
		}
	MPI_Finalize();
}

bool is_pow_2(unsigned int val)
{
	if (val < 1)
		return false;
	return !(val & (val - 1));
}

bool is_prime(unsigned int val)
{
	if (val < 2)
		return false;
	unsigned int n = static_cast<unsigned int>(std::sqrt(val)) + 1;
	for (unsigned int i = 2; i < n; ++i)
	{
		if (val % i == 0)
		{
			return false;
		}
	}
	return true;
}
