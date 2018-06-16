/*
 * ASSEGNAMENTO1-5:
 *  Scrivere una funzione mystrcat con la seguente segnatura:
 *    char *mystrcat(char *, ...);
 *  che prenda un numero di stringhe variabili e che concateni 
 *  tutte le stringhe alla prima con lo stesso comportamento 
 *  della funzione di libreria 'strcat' (man 3 strcat).
 *  
 * 
 * COMPILAZIONE:
 *  gcc -std=c99 -Wall -g ./varargscat.c -o varargscat
 *
 */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

char *mystrcat(char *first, ...) {
    va_list ap;
    va_start(ap, first);

    char *s = NULL;
    while ((s = va_arg(ap, char*))!=NULL)
		strncat(first, s, strlen(s));
    
    va_end(ap);
    return first;
}

int main() {
    char prima[1024] = "prima";
    printf("%s\n", mystrcat(prima, "seconda", "terza lunga", "quarta", "quinta", "ultima!", NULL));

    return 0;

}
