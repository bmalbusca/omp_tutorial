#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

#define NPOINTS 1000 
#define MXITR 1000 


struct d_complex
    {
        double r; 
        double i; 
    };
struct d_complex c; 
int numoutside = 0;


int testpoint(struct d_complex c){ 
    struct d_complex z;
    int iter, part_numoutside=0; 
    double temp;
    z=c;

    for (iter=0; iter<MXITR; iter++){
            temp = (z.r*z.r)-(z.i*z.i)+c.r; z.i = z.r*z.i*2+c.i;
            z.r = temp;
            if ((z.r*z.r+z.i*z.i)>4.0) {
                part_numoutside++;
                break; 
            }
    }
    return part_numoutside;
}




int main(){ 
    int i, j;
    double area, error; 
    double eps = 0.00001; 
    #pragma omp parallel for default(shared)  private(c,j) firstprivate(eps) reduction(+:numoutside) 
    for (i=0; i<NPOINTS; i++) {
        for (j=0; j<NPOINTS; j++) {
            c.r = -2.0+2.5*(double)(i)/(double)(NPOINTS)+eps; 
            c.i = 1.125*(double)(j)/(double)(NPOINTS)+eps; 
            numoutside += testpoint(c);

        }
    }
    area=2.0*2.5*1.125*(double)(NPOINTS*NPOINTS- numoutside)/(double)(NPOINTS*NPOINTS);
    error=area/(double)NPOINTS;
    printf("Area=%lf (error=%lf)\n", area,error);
    return 0;
}
