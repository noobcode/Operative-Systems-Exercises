/*
 * ASSEGNAMENTO1-4:
 *  Scrivere un programma che dato un array di un certo numero di elementi 
 *  esegua la somma o il prodotto di tutti gli elementi dell'array a seconda
 *  del parametro passato come input al programma (-s o -m).
 *  Scrivere il programma in modo generico utilizzando un puntatore a funzione.

 * 
 * COMPILAZIONE:
 *  gcc -std=c99 -Wall -g ./cmdlineparsing.c -o cmdlineparsing
 *
 */

#include <stdio.h>
#include <string.h>

void print_usage(const char *programname) {
    printf("usage:\n %s -s\n %s -m\n", programname, programname);
}

long somma(long *V, size_t size) {
    long s = 0;
    for(size_t i=0;i<size;++i)
	s += V[i];
    return s;
}
long prod(long *V, size_t size) {
    long s = 1;
    for(size_t i=0;i<size;++i)
	s *= V[i];
    return s;
}

// definisco il tipo funzione che mi serve
typedef long (*function_t)(long *, size_t);

int main(int argc, char *argv[]) {

    if (argc == 1 || argc > 2) {
	print_usage(argv[0]);
	return EXIT_FAILURE;
    }

    size_t size = 10;
    long V[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    function_t fptr = NULL;  // puntatore a funzione di tipo function_t
                              
    ++argv;
#if 0
    if (strncmp(argv[0],"-s",2)==0) fptr = somma;
    if (strncmp(argv[0],"-m",2)==0) fptr = prod;
#else // modo alternativo senza usare strncmp
    if (argv[0][0]=='-' && argv[0][1]=='s') fptr = somma;
    if (argv[0][0]=='-' && argv[0][1]=='m') fptr = prod;
#endif

    if (fptr == NULL) {
	printf("argomento non valido\n");
	--argv;
	print_usage(argv[0]);
	return EXIT_FAILURE;
    }
    printf("Risultato: %ld\n", fptr(V,size));

    return 0;
}
