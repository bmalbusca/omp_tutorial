#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 10

void fill_rand(int size, double*array ){

    for(int i=0;i<N;++i){
        array[i]=i+1;
    }
}

double  Sum_array(int size, double*array){

    double sum=0;
    for(int i=0;i<N;++i){
         sum += array[i];
    }
    return sum ;
}


int main()
{
    double *A, sum,check_sum, runtime; int flag = 0;
    A = (double *) malloc(N*sizeof(double));
    runtime = omp_get_wtime();
    fill_rand(N, A); // Producer: fill an array of data
    
    #pragma omp parallel sections //should be not dependent blocks, each section will run in parallel and wait at the end (fork join) 
    {
        #pragma omp section
        {
           check_sum= (N*(N+1.0))/2.0; // Producer: check sum
        }    
        #pragma omp section
        {
            sum = Sum_array(N, A); // Consumer: sum the array 
        }
    }
    runtime = omp_get_wtime() - runtime;
    printf(" In %lf secs, The sum is %lf (check %lf) \n",runtime,sum, check_sum); 

}
