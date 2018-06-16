/*
 * ASSEGNAMENTO2-3:
 *  Scrivere un programma che prende in ingresso come parametro un nome di un file 
 *  (ad esempio input.txt). Il file è un file testuale che contiene un certo numero di righe.
 *  Leggere tutte le righe del file e costruire un'unica stringa in memoria data dalla 
 *  concatenazione di ogni striga del file separate da uno spazio.
 *  Scrivere la stringa cosi ottenuta in un file che ha lo stesso nome  
 *  del file in input ma con estensione ".log" (per il nostro esempio il nome sarà input.log). 
 *
 * COMPILAZIONE:
 *  gcc -std=c99 -Wall -Werror concat.c -o concat
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// macro di utilità per controllare errori in chiamate di libreria
#define CHECK_CALL_PTR(X, str)	\
    if ((X)==NULL) {		\
      perror(#str);		\
      exit(EXIT_FAILURE);	\
    }
    
#define MAX_LINE 512

//prototype per strdup, non e' nello standard C99
char *strdup(const char *s);

int main(int argc, char *argv[]) {
    if (argc != 2) {
	fprintf(stderr, "usa: %s <nome-file>\n", argv[0]);
	exit(EXIT_FAILURE);
    }
    
    char *filename = strdup(argv[1]);
    FILE *fp;
    CHECK_CALL_PTR(fp = fopen(filename,"r"), "fopen");
    char line[MAX_LINE];
    char *membuf;
    CHECK_CALL_PTR( membuf = malloc(2*MAX_LINE*sizeof(char)) , "malloc");
    size_t allocatedsize = 2*MAX_LINE;
    size_t size = allocatedsize;
    char *pmem = membuf;
    while(fgets(line, MAX_LINE, fp) != NULL) { // fgets legge al piu' MAX_LINE-1 caratteri. 
	                                       // la lettura si interrompe se si incontra EOS of '\n'
	const size_t linelen = strlen(line);
	strncpy(pmem,line, linelen);
	pmem += linelen;
	if (*(pmem-1) == '\n') *(pmem-1) = ' ';  // rimuovo \n se c'e'
	size -= linelen;
	if (size < MAX_LINE) {  // devo allargare il buffer per la prossima linea
	    size_t offset = pmem - membuf;
	    CHECK_CALL_PTR(membuf=realloc(membuf, allocatedsize+(2*MAX_LINE*sizeof(char))), "realloc");
	    allocatedsize += 2*MAX_LINE*sizeof(char);
	    size+=2*MAX_LINE*sizeof(char);
	    pmem = membuf+offset;
	}
    }    
    fclose(fp);
    memset(pmem, '\0', allocatedsize-(pmem-membuf));
    char * r = strstr(filename, ".");
    if (r) *r='\0';
    char *newfilename;
    CHECK_CALL_PTR(newfilename=calloc(1, (strlen(filename)+4+1)*sizeof(char)), "calloc");
    strncat(newfilename, filename, strlen(filename));
    strncat(newfilename, ".log", 4);
    CHECK_CALL_PTR(fp = fopen(newfilename, "w+"), "fopen");
    if (fprintf(fp, "%s", membuf) != strlen(membuf)) {
	perror("fprintf");
	exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}
