#include <stdio.h>
#include <stdlib.h>

#define CHECK_EQ(X, val, str)					      \
    if ((X)==val) {						      \
	perror(#str);						      \
	printf("ERRORE ALLA LINEA %d FILE %s\n", __LINE__, __FILE__); \
	exit(EXIT_FAILURE);					      \
    }
#define CHECK_NEQ(X, val, str)					      \
    if ((X)!=val) {						      \
	perror(#str);						      \
	printf("ERRORE ALLA LINEA %d FILE %s\n", __LINE__, __FILE__); \
	exit(EXIT_FAILURE);					      \
    }

#define dimN 16
#define dimM  8

#define ELEM(Mat,i,j)	 Mat[i*dimN+j]
#define PRINTMAT(MAT, matname)				\
    do {						\
	printf("Stampo la matrice %s:\n",#matname);	\
	for(size_t i=0;i<dimN;++i) {			\
	    for(size_t j=0;j<dimM;++j)			\
		printf("%4ld ", ELEM(MAT,i,j));		\
	    printf("\n");				\
	}						\
    }while(0)
	    
int main(int argc, char *argv[]) {

    long *M;
    CHECK_NEQ(M=malloc(dimN*dimM*sizeof(long)), NULL, "malloc");
    
    for(size_t i=0;i<dimN;++i)
	for(size_t j=0;j<dimM;++j)			
	    ELEM(M,i,j) = i+j;
    
    PRINTMAT(M, M);

    return 0;
}
