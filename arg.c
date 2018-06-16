#include<stdio.h>
#include<string.h>

typedef long (*function_t)(long *, size_t);

long somma(long* a, size_t n){
	int i; 
	long somma = 0;
	for(i = 0; i < n; i++)
		somma += a[i];
	return somma;
}

long prodotto(long* a, size_t n){
	int i; 
	long prod = 1;
	for(i = 0; i < n; i++)
		prod *= a[i];
	return prod;
}

int main(int argc, char* argv[]){
	long a[] = {2,2,2,2,2};
	function_t ptrsomma = somma;
	function_t ptrprodotto = prodotto;
	
	if(strcmp(argv[1],"-s") == 0)
		printf("somma : %ld\n",ptrsomma(a,5));
	else if(strcmp(argv[1],"-m") == 0)		
		printf("prodotto : %ld\n",ptrprodotto(a,5));
	return 0;
}
