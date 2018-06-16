#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "liblist.h"
#include <mcheck.h>
#define LEN 101
#define EOF (-1)
#define CHECKFAILURE(p, str)	if(!(p)) { \
								printf("errore alla linea %d del file %s\n", __LINE__, __FILE__); \
								perror(str); \
								exit(EXIT_FAILURE); }
					
								
int main(int argc, char* argv[]){
	if(argc != 2){
		perror("argc");
		exit(EXIT_FAILURE);
	}
	mtrace(); // mtrace
	char* str = malloc(LEN * sizeof(char));
	char* filename = argv[1];
	list_t* l = NULL;
	FILE *f;
	
	CHECKFAILURE(f = fopen(filename, "r"), "fopen");
	CHECKFAILURE(l = createList(), "createList");
	
	while(fscanf(f, "%s", str) != EOF){
		insertList(l, str);
	}
	
	printList(l);
	destroyList(l);
	fclose(f);
	free(str);
	muntrace(); // muntrace
	return 0;
}
