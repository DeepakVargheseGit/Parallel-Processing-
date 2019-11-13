#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <sys/time.h>

int main(argc,argv)
int argc;
char *argv[];
{
    int done = 0, n, myid, numprocs, i;
   // double PI25DT = 3.141592653589793238462643;
    double pi_final, pi, act_pi,org_pi,width, sum, x,fo,fn,pi_diff,denominator;
    struct timeval tvBegin,tvEnd; // Used to get the start and end time values

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);

        if (myid == 0)
        {
            printf("Enter the number of intervals: "); //The interval is given here
            scanf("%d",&n);
        }
        gettimeofday(&tvBegin,NULL); //Used to calculate the start time

        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD); //Used to broadcast message from root processor to the rest

        width = 1.0 / (double) n; // used for normal interval calculation - taken from professors notes
        sum = 0.0;

        fo = ((double )4/(1+0));  // used to calculate the values for fo
        fn = ((double )4/(1+1));  // used to calculate the values for fn

        for (i = myid + 1; i <= n; i += numprocs)  // loops for all processors to calculate the pi value
        {
            x = width * ((double)i - 0.5);
            sum = 4.0 / (1.0 + x*x);
            org_pi += sum;

            if(i%2==0)
                {
                  sum = sum*2;
                }
              else
                {
                  sum = sum*4;
                }
                pi_final = pi_final+sum;
        }

        org_pi*= width;  // Stores the original pi value

        MPI_Reduce(&pi_final, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD); //Used to reduce all values on processes to a single value

        MPI_Reduce(&org_pi, &act_pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

        if (myid == 0)  //Final value for calculated pi value is done here
       {
             pi = pi + fo - fn ;
        denominator = (3*n);

        pi = pi/denominator;

        gettimeofday(&tvEnd,NULL); //Used to calculate the end time

        printf("\n The Original Pi value is %.20f",act_pi); // The original pi value
        printf("\n The Final Pi value is %.20f",pi); //The calculated pi value using simpsons rule

        pi_diff = fabs(pi - act_pi); //Calculates difference between original and calculated pi value

        printf("\n The difference Pi value is %.20f",pi_diff);

        printf("\n The communication time is %f\n",(float)(tvEnd.tv_usec-tvBegin.tv_usec)/100.0); // Shows the time total time taken
       }

    MPI_Finalize(); //Terminates the MPI environment
    return 0;
}
