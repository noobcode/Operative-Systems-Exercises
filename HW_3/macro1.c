//
// Una semplice macro che somma gli elementi di un vettore
//
#include <stdio.h>

#define SOMMAV(N,V,sum)	   \
    do {		   \
     sum=0;		   \
     for(long i=0;i<N;++i) \
 	 sum += V[i];	   \
    } while(0)


int main() {
    long V[10] = { 0,1,2,3,4,5,6,7,8,9};
    long sum;
    SOMMAV(10,V,sum);
    printf("Somma = %ld\n", sum);
    return 0;
}
