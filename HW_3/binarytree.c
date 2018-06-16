/*
 * ASSEGNAMENTO2-4:
 * Realizzare l'esercizio 7 (dell'Assegnamento1) che costruisce un albero binario di ricerca da un 
 * array di N elementi con le seguenti modifiche:
 *  - gli elementi dell'array devono essere letti da file, il formato del file è:
 *
 *  [+|-]numero-intero   (un numero per riga che puo' avere o meno il segno '+' o '-' davanti) 
 *
 *  - le funzioni 'buildTree' 'printInOrder' 'getMin' 'getMax' 'deleteTree' devono essere 
 *    implementate in file separati (la definizione in un file .h e l'implementazione in un file .c). 
 *
 * Creare il codice oggetto dei due file .c e linkarli insieme in un unico eseguibile.
 *
 * COMPILAZIONE:
 *   gcc -std=c99 -Wall -I. binarytreelib.c -c -o binarytreelib.o
 *   gcc -std=c99 -Wall -I. binarytree.c -c -o binarytree.o
 *   gcc binarytree.o binarytreelib.o -o binarytree
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <binarytree.h>

// lunghezza massima per una singola lettura di linea con fgets
#define MAX_LINE 512

// macro di utilità per controllare errori in chiamate di libreria
#define CHECK_CALL_PTR(X, str)	\
    if ((X)==NULL) {		\
      perror(#str);		\
      exit(EXIT_FAILURE);	\
    }


/* legge la lista di interi contenuta nel file di testo passato in input,
   crea l'array numbers e restituisce il numero di elementi letti 
*/
size_t getNumbersFromFile(const char *filename, long **numbers) {
    FILE *fp;
    CHECK_CALL_PTR(fp = fopen(filename, "r"), "fopen");
    
    size_t size = 100, current_size=0;
    CHECK_CALL_PTR(*numbers = malloc(size*sizeof(long)), "malloc");
    long *num = *numbers;
    char line[MAX_LINE];
    while(fgets(line, MAX_LINE, fp) != NULL) { 
	long n = strtol(line, NULL, 10);
	if (current_size == size) {
	    CHECK_CALL_PTR(*numbers = realloc(*numbers, (size+100)*sizeof(long)), "realloc");
	    size += 100;
	}
	num[current_size++] = n;
    }
    fclose(fp);	
    return current_size;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
	fprintf(stderr, "use: %s <nome-file>\n", argv[0]);
	return EXIT_FAILURE;
    }
    const char *filename = argv[1];
    long array_size = 0;
    long *array = NULL;
    
    array_size = getNumbersFromFile(filename, &array);

    struct node_t *root = NULL;    
    for(long i=0; i<array_size; ++i)
	root = buildTree(array[i], root);

    printf("Min: %ld\n", getMin(root));
    printf("Max: %ld\n", getMax(root));

    printInOrder(root);
    printf("\n");

    deleteTree(root);
    free(array);

    return 0;
}
