#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int isprime(int n) {
  int i,squareroot;
  if (n>10) {
    squareroot = (int) sqrt(n);
    for (i=3; i<=squareroot; i=i+2)
      if ((n%i)==0)
        return 0;
    return 1;
  }
  else
    return 0;
}
int main(int argc, char** argv) {
    int ntasks,               	/* total number of tasks in partitiion */
	rank,                 	/* task identifier */
	pc,       		/* prime counter */
	pcsum,                	/* number of primes found by all tasks */
	foundone, 		/* most recent prime found */
	maxprime,             	/* largest prime found */
	mystart,              	/* where to start calculating */
      	stride;               	/* calculate every nth number */

    long long int n, limit;
    int rc,tag=1;
    double start_time,end_time;

    sscanf(argv[1],"%llu",&limit);
    //printf("Starting. Numbers to be scanned= %lld\n",limit);
    pc=4;/* Assume (2,3,5,7) are counted here */

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    start_time = MPI_Wtime();   /* Initialize start time */
    mystart = 11;       /* Find my starting point - must be odd number */
    stride = 2;          /* Determine stride, skipping even numbers */
    pc=0;                       /* Initialize prime counter */
    foundone = 0;               /* Initialize */
    
    if (rank == 0) {
   	//printf("Using %d tasks to scan %lld numbers\n",ntasks,limit);
   	pc = 4;                  /* Assume first four primes are counted here */
   	for (n=mystart; n<=limit; n=n+stride) {
      		if (isprime(n)) {
         		pc++;
         		foundone = n;
         		//printf("%d\n",foundone);
         	}
      	}
   	MPI_Reduce(&pc,&pcsum,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
   	MPI_Reduce(&foundone,&maxprime,1,MPI_INT,MPI_MAX,0,MPI_COMM_WORLD);
   	end_time=MPI_Wtime();
   	printf("Done. Largest prime is %d Total primes %d\n",maxprime,pcsum);
   	//printf("Wallclock time elapsed: %.2lf seconds\n",end_time-start_time);
   }    

   if (rank > 0) {
   	for (n=mystart; n<=limit; n=n+stride) {
      		if (isprime(n)) {
         		pc++;
         		foundone = n;
         	}
      	}
   	MPI_Reduce(&pc,&pcsum,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
   	MPI_Reduce(&foundone,&maxprime,1,MPI_INT,MPI_MAX,0,MPI_COMM_WORLD);
   }
    MPI_Finalize();
    return 0;
}
