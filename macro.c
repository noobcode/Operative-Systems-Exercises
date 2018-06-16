#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define dimN 16
#define dimM  8
#define ELEM(M,i,j) M[dimN*i + j]
#define PRINTMAT(M) for(int i = 0; i < dimN; i++){ \
						for(int j = 0; j < dimM; j++) \
							printf("%ld ",M[dimN*i +j]);\
						printf("\n");}
						
#define CHECK_EQ(P, a, str)	if((P) == a){	 perror(str); \
								printf("ERRORE ALLA LINEA %d DEL FILE %s\n", __LINE__, __FILE__); \
							exit(EXIT_FAILURE);}			

#define CHECK_NEQ(P, a, str)	if((P) != a){	 perror(str); \
								printf("ERRORE ALLA LINEA %d DEL FILE %s\n", __LINE__, __FILE__); \
							exit(EXIT_FAILURE);}
								
int main() {
    long *M = malloc(dimN*dimM*sizeof(long)); //M = NULL;
    CHECK_NEQ(M, NULL, "malloc");
    assert(M);
    for(size_t i=0;i<dimN;++i)
	for(size_t j=0;j<dimM;++j)			
	    ELEM(M,i,j) = i+j;
    
    PRINTMAT(M);
    return 0;
}
