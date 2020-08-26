#include <stdio.h>
#include <omp.h>


int main(void){
    
    #pragma omp parallel 
    {
        int ID =0;
        ID = omp_get_thread_num();
        printf("hello from %i \n", ID);
    }
    return 0;
}
