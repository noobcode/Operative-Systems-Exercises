/* 
 * ASSEGNAMENTO2-1:
 *  Realizzare l'equivalente del comando Unix "cat" senza opzioni.
 *  (cat file1 file2 ...)
 *
 * COMPILAZIONE:
 *  gcc -std=c99 -Wall -Werror mycat.c -o mycat
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <getopt.h>


// macro di utilità per controllare errori in chiamate di libreria
#define CHECK_CALL_PTR(X, str)	\
    if ((X)==NULL) {		\
      perror(#str);		\
      exit(EXIT_FAILURE);	\
    }
    
// lunghezza massima per una singola lettura di linea con fgets
#define MAX_LINE 512

int main(int argc, char *argv[]) {
    if (argc == 1) {
	fprintf(stderr, "usa: %s <nome-file1> [<nome-file2> ...]\n", argv[0]);
	exit(EXIT_FAILURE);
    }
    char line[MAX_LINE];
    int i=1;
    // per ogni file 
    while(argv[i] != NULL) {
	FILE *fp;             // file pointer
	char *filename = argv[i];
	CHECK_CALL_PTR(fp = fopen(filename,"r"), "fopen");

	while(fgets(line, MAX_LINE, fp) != NULL) { 
	    if (fputs(line, stdout) == EOF) {
		perror("fputs");
		return EXIT_FAILURE;
	    }
	}
	fclose(fp);
	++i;
    }

    return EXIT_SUCCESS;
}
