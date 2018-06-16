/*
 * ASSEGNAMENTO1-6:
 *  Scrivere un programma che effettua il parsing della linea di comando riconoscendo 
 *  le seguenti opzioni: -n <numero> -s <stringa> -m <altro-numero> -h. 
 *  Il programma dovrà stampare le opzioni riconosciute con il relativo argomento. 
 *  L'opzione -h non ha argomento  e corrisponde al messaggio di help. 
 *  Se e' presente l'opzione -h dovra' essere stampato solo il messaggio di usage.
 *  Se ci sono opzioni non riconosciute queste dovranno essere stampate.

 *  Testare il programma con:
 *    - cmdlineparsing -n 10 -h  (deve stampare solo il messaggio di usage) 
 *    - cmdlineparsing -n 10 -m 11 -s 'ciao mondo'
 *    - cmdlineparsing -n 10 -k 12   (k e' una opzione non riconosciuta)
 *    - cmdlineparsing ----n 10 -s-s 'ciao mondo' (deve stampare -n: 10 -s: -s)
 *    - cmdlineparsing -n10 -m11 -s'ciao mondo' (deve stampare gli argomenti corretti)
 *    - cmdlineparsing -n -m 11   (deve stampare un messaggio di errore per -n)
 * 
 * COMPILAZIONE:
 *  gcc -std=c99 -Wall -g ./cmdlineparsing.c -o cmdlineparsing
 *
 */

#include <stdio.h>
#include <stdlib.h>

void print_usage(const char *programname) {
    printf("usage: %s -n <num> -m <num> -s <string> -h\n", programname);
}

int main(int argc, char *argv[]) {
    char *programname = argv[0];

    if (argc == 1) {
	printf("nessun argomento passato al programma\n");
	return 0;
    }

    char c;
    char foundn = 0, foundm=0, founds=0;
    long narg = -1, marg = -1;
    char *sarg = NULL;
    while(--argc > 0) 
	if ((*++argv)[0] == '-') {
	    while((c = *++argv[0]) == '-'); // alternativamente: while((c = argv[0][1]) == '-');
      	                                    // Che differenza c'e' tra *++argv[0] e (*++argv)[0] ??
	    switch(c) {
	    case 'n': {
		foundn = 1;
		if (argv[0][1] == '\0') {
		    ++argv, --argc;
		    if (argv[0] == NULL) {
			foundn = 0;
			printf("argomento n non valido\n");
		    } else 
			narg = atol(argv[0]);		    
		} else narg = atol(&argv[0][1]);
	    } break;
	    case 'm': {
		foundm = 1;
		if (argv[0][1] == '\0') {
		    ++argv, --argc;
		    marg = atol(argv[0]);
		}  else  marg = atol(&argv[0][1]);

	    } break;
	    case 's': {
		founds = 1;
		if (argv[0][1] == '\0') {
		    ++argv, --argc;
		    sarg = argv[0]; // attenzione questo e' rischioso, conviene usare strdup !
		} else sarg = &argv[0][1];
		if (sarg == NULL) {
		    founds = 0;
		    printf("argomento s non valido\n");
		}
	    } break;
	    case 'h': {
		print_usage(programname);
		return 0;
	    } break;
	    default : 
		printf("argomento %c non riconosciuto\n", c);	    
	    }
	}

    if (foundn) 
	printf("-n: %ld\n", narg);
    if (foundm) 
	printf("-m: %ld\n", marg);
    if (founds) 
	printf("-s: %s\n", sarg);
    

    return 0;

}
