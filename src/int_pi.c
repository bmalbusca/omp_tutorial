/* numerical integration - pi
 * int_{1}^{0} frac{4.0}{1+x^2} dx = pi
 *
 * can be aprox to a sum(F(x_{i} delta_x))
 */
 
#include <stdio.h>
#include <omp.h> 

#define STEPS 100000


static long double delta= 1.0/(double)STEPS;
static int steps =STEPS; 

//int part_sum =0;
//#pragma omp  threadprivate(part_sum) //this pragma works on a global/static variable


void pi_reduction(){
    
    long double sum=0.0, xi=0.0;

    //reduction already initializes the variable to reduce
    #pragma omp  parallel for firstprivate(xi)  reduction(+:sum) schedule(guided)

    
    for(int i=0; i< steps; ++i){
        xi= (i+0.5)*delta;
        sum += (4.0/(1.0+(xi*xi)))*delta; 
    }


    printf(" reduction version (%i threads):  pi value = %Lf\n", omp_get_max_threads(), sum);


}


void pi_atomic(){
    
    long double sum=0.0, xi=0.0;

    #pragma omp parallel for firstprivate(xi) shared(sum) schedule(guided) 
    for(int i=0; i< steps; ++i){
        xi= (i+0.5)*delta;
        #pragma omp atomic
        sum += (4.0/(1.0+(xi*xi)))*delta; 
      
    }

    printf(" atomic version (%i threads) : pi value = %Lf\n", omp_get_max_threads(), sum);

}

void pi_tasks(){
    long double sum=0.0;
    int nthreads=0;
    // xi comes from outside of the parallel section and thus is implicitly shared in the parallel section
    #pragma omp parallel  
    { 
        #pragma omp master
        {
            for(int i=0; i<STEPS; ++i){
                #pragma omp task shared(sum)  // sum variable need to be explicit shared  
                {   
                     long double xi=(i+0.5)*delta; 
                     long double part_sum = (4.0/(1.0+(xi*xi)))*delta;
                     #pragma omp atomic
                     sum += part_sum;  
   
                }
            }
            #pragma omp taskwait  
        }
    
        nthreads = omp_get_num_threads();    
    }

    printf(" tasks version (%i threads): pi value = %Lf\n",nthreads, sum);

}

int main(void){
    

        pi_reduction();
        pi_atomic();
        pi_tasks();
    
    return 0; 
}
