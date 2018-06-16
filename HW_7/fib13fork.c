/* Esercizio 1:
 * Calcola i primi 13 numeri di Fibonacci ricorsivamente ed utilizzando
 * solo processi. LSO 2015 Corso A.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

static void doFib(int n, int doPrint);

int main(int argc, char *argv[]) {
    // questo programma puo' calcolare i numeri di Fibonacci solo fino a 13.  
    const int NMAX=13;
    int arg;
    
    if(argc != 2){
	fprintf(stderr, "Usage: %s <num>\n", argv[0]);
	return EXIT_FAILURE;
    }
    
    arg = atoi(argv[1]);
    if(arg < 0 || arg > NMAX){
	fprintf(stderr, "num deve essere compreso tra 0 e 13\n");
	return EXIT_FAILURE;
    }
    
    doFib(arg, 1);
    return 0;
}

/* 
 * Calcola ricorsivamente il numero di Fibonacci dell'argomento 'n'.
 * La soluzione deve forkare un nuovo processo che esegue una sola 
 * chiamata di 'doFib'.
 * Se doPrint e' 1 allora la funzione lo stampa, altrimenti il
 * numero calcolato va passato al processo padre.
 */
static void doFib(int n, int doPrint) {
    int m = n;
    if (n>2) {    
	if (fork() == 0) doFib(n-1, 0);
	else {
	    if (fork() == 0) doFib(n-2, 0);	    
	    else {
		int status;
		if (wait(&status) == -1) {
		    perror("wait");
		    return;
		}
		m = WEXITSTATUS(status);
		if (wait(&status) == -1) {
		    perror("wait");
		    return;
		}
		m += WEXITSTATUS(status);
	    }
	}
    }
    if (doPrint)  printf("m=%d\n", m);
    return exit(m);
}


