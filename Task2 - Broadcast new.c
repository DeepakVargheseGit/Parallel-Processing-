#include "mpi.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
int main( int argc, char *argv[] )
{
    int rank, size;

    int i,arr_size,val;
    int *array1,*array2;
    //int num_of_int=0;

    struct timeval tvBegin,tvEnd,tvDiff; // Used to get the start and end time values

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    if(rank == 0)
    {
      printf("\n Enter the size of the array : "); //The array size is given here.
      scanf("%d",&arr_size);
    }

    MPI_Bcast(&arr_size,1,MPI_INT,0,MPI_COMM_WORLD); //Used to broadcast message from root processor to the rest

    array1 = (int *)malloc(arr_size*sizeof(int)); //Dynamically allocating the array
    array2 = (int *)malloc(arr_size*sizeof(int)); //Dynamically allocating the array

    for ( i=0 ; i < arr_size ; ++i ) //Loop to calculate and store array values
    {
        val = val+5;
        array1[i] = val;
        array2[i] = 5;
        printf("The original array values for processor %d are %d \n",rank,array1[i]); //Shows the original array values
    }
    gettimeofday(&tvBegin,NULL); //Checks the start time here

      MPI_Bcast(&(*array1),arr_size, MPI_INT,0, MPI_COMM_WORLD); //Used to broadcast message from root processor to the rest

    gettimeofday(&tvEnd,NULL); //Checks the end time here

    for ( i= 0;i < arr_size ; i++)
    {
        printf("The final array values for processor %d are %d \n",rank,array1[i]); //Displays the final array values
    }

   if(rank==0)
    {
     printf("The collective  communication time is %f\n",(float)(tvEnd.tv_usec-tvBegin.tv_usec)/100.0); //Calculates the final time
     }

     free(array1); //frees the allocated memory
     free(array2);
     MPI_Finalize(); //Terminates the MPI environment
  }
