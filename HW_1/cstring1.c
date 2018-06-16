/*
 * ASSEGNAMENTO1-2:
 *  Data una stringa con caratteri minuscoli e maiuscoli,
 *  trasformarla in una stringa con tutti i caratteri maiuscoli.
 *  NOTE: è possibile utilizzare le funzioni di libreria islower
 *        e toupper definite in ctype.h. Vedere man 3 islower !
 * 
 * COMPILAZIONE:
 *  gcc -std=c99 -Wall -g ./cstring1.c -o cstring
 *
 */


#include <stdio.h>
#include <ctype.h>

int main() {
    char str[]="Questa e' una Stringa non tanto Lunga che CONtiene 123 !";
    char *p = str;

    while(*p != '\0') { 
	// *p++ = (islower(*p)?toupper(*p):*p); // DOMANDA: perche' questo è sbagliato ? 
        *p = (islower(*p)?toupper(*p):*p); 
	++p;
    }    
    printf("%s\n", str);
    return 0;
}
