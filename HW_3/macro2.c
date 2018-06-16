#include <stdio.h>
#include <stdlib.h>

#define dimN 16
#define dimM  8

#define ELEM(Mat,i,j)	 Mat[i*dimN+j]
#define PRINTMAT(MAT)				 \
    do {					 \
     printf("Stampo la matrice %s:\n", #MAT);     \
     for(size_t i=0;i<dimN;++i) {		 \
	 for(size_t j=0;j<dimM;++j)		 \
	     printf("%4ld ", ELEM(MAT,i,j));	 \
	 printf("\n");				 \
     }						 \
    }while(0)
	    
int main(int argc, char *argv[]) {

    long *M = malloc(dimN*dimM*sizeof(long));
    
    for(size_t i=0;i<dimN;++i)
	for(size_t j=0;j<dimM;++j)			
	    ELEM(M,i,j) = i+j;
    
    PRINTMAT(M);

    return 0;
}
