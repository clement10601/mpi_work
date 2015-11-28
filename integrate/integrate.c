#include <stdio.h>
#include <math.h>
#include <mpi.h>
#define PI 3.1415926535

int main(int argc, char** argv) {
    long long i, num_intervals;
    double rect_width, area, sum, x_middle;
    double totalsum;
    int rank, ntasks;
    double start_time,end_time;

    sscanf(argv[1],"%llu",&num_intervals);
    rect_width = PI / num_intervals;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    start_time = MPI_Wtime();
    sum = 0;
    for(i = 1; i < num_intervals + 1; i++) {
    	/* find the middle of the interval on the X-axis. */
    	x_middle = (i - 0.5) * rect_width;
    	area = sin(x_middle) * rect_width;
        sum = sum + area;
    }
    MPI_Reduce(&sum,&totalsum,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
    if(rank==0)
    {
	printf("The total area is: %f\n", (float)totalsum);
    }

    MPI_Finalize();
    return 0;
}
