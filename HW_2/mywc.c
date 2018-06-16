/*
 * ASSEGNAMENTO2-5:
 * Scrivere un programma che realizzi l'equivalente del comando Unix "wc" 
 * limitatamente alle opzioni -l e -w (man 1 wc). 
 * L'opzione -l restituisce il numero di linee del file, l'opzione -w restituisce il numero
 * di parole nel file. Se non vengono passate opzioni, il programma stampa sia il numero
 * di linee che il numero di parole dei/dei file i cui nomi sono passati come argomento 
 * (wc [-l -w] file1 [file2 file3 ....])
 * 
 * COMPILAZIONE:
 *  gcc -std=c99 -Wall -Werror mywc.c -o mywc
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
#define DEFAULT_MAX_LINE 2048
// utilizzato dalla getopt
extern int optind;

// ritorna il numero di parole contenute nella stringa buffer
int wcount(const char buf[]) {
    char *p = (char*)&buf[0];
    int count = 0, flag=0;
    // togliamo tutti gli spazi ed i caratteri di controllo
    // iniziali in modo da iniziare con un carattere
    while(*p != '\0' && (iscntrl(*p) || isspace(*p))) ++p;

    while(*p != '\0') { 
	if (isspace(*p)) {  // se ho uno spazio e se il carattere precedente 
	                    // non e' ne uno spazio ne un carattere di controllo....
	    if ( !isspace(*(p-1)) && !iscntrl(*(p-1)) ) { 
		++count;
		flag = 0;
	    }
	} else flag=1;  // ho iniziato una nuova parola
	++p;
    }
    return (flag) ? count+1:count;
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
	fprintf(stderr, "usa: %s [-l -w -m <num> ] <nome-file> [<nome-file> ...]\n", argv[0]);
	fprintf(stderr, " -l conta il numero di linee\n");
	fprintf(stderr, " -w conta il numero di parole\n");
	fprintf(stderr, " -m <num> setta la lunghezza massima di una linea a <num>\n");
	exit(EXIT_FAILURE);
    }
    int countline = 0, countword = 0;
    char opt;
    long max_line = DEFAULT_MAX_LINE;
    // parsing della command line
    while((opt = getopt(argc, argv, "lwm:")) != -1) {
	switch(opt) {
	case 'l': {
	    countline = 1;
	} break;
	case 'w': {
	    countword = 1;
	} break;
	case 'm': {
	    max_line = strtol(optarg, NULL, 10);
	} break;
	default: ;
	}
    }

    // se non ci sono opzioni considero sia -l che -w come abilitate
    if (countword + countline == 0) { countline=1; countword=1; }

    char *line = malloc(max_line*sizeof(char)); // buffer

    // per ogni file 
    while(argv[optind] != NULL) {
	FILE *fp;             // file pointer
	long numline,numword; // contatori
	
	char *filename = argv[optind];
	CHECK_CALL_PTR(fp = fopen(filename,"r"), "fopen");

	numline=numword=0;
	while(fgets(line, max_line, fp) != NULL) { 
	    if (line[0] != '\n' && line[strlen(line)-1] == '\n') ++numline;
	    if (countword) numword += wcount(line);
	}
	fclose(fp);
	if (countline && !countword) printf("  %ld %s\n", numline, argv[optind]);
	if (!countline && countword) printf("  %ld %s\n", numword, argv[optind]);
	if (countline && countword)  printf("  %ld  %ld %s\n", numline, numword, argv[optind]);
	++optind;
    }
    free(line);

    return EXIT_SUCCESS;
}
