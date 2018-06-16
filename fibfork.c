#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#define CHECK_MENO_1(p,str) if((p) == -1){ perror(str); exit(EXIT_FAILURE); 

/* calcola ricorsivamente il fibonacci dell'alrgomento 'n'. 
 la soluzione deve forkare un nuovo processo che esegue una sola chiamata di 'doFib'. 
 se doPrint è 1 allora la funzione lo stampa, altrimenti il numero calcolato va passato al processo padre. */
static void doFib(int n, int doPrint){
	
}

int main(int argc, char* argv[]){
	// può calcolare i num di fibonacci solo fino a 13.
	const int NMAX = 13;
	int arg;
	
	if(argc != 2){
		fprintf(stderr,"usage: %s <num>\n",argv[0]);
		return EXIT_FAILURE;
	}
	arg = atoi(argv[1]);
	if(arg < 0 || arg > NMAX){
		fprintf(stderr,"numero troppo grande o piccolo\n");
		return EXIT_FAILURE;	
	}
	doFib(arg,1);
	return 0;
}
