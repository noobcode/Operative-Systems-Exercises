#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <Lista.h>

#define MAX_LINE 1024

// ********************************************************
// WARNING: strtok e' pericolosa leggere bene il manuale !!
// ********************************************************
void tokenizeAndInsert(list_t *L, char *line) {
    char* token = strtok(line, " ");
    while (token) {
	CHECK_NEQ(insertList(L, token), 0, "insertList");
	printf("token: %s\n", token);
	token = strtok(NULL, " ");
    }
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
	fprintf(stderr, "usa: %s <nome-file>\n", argv[0]);
	exit(EXIT_FAILURE);
    }
    char *filename = argv[1];
    FILE *fp;        
    CHECK_EQ(fp = fopen(filename, "r"), NULL, "fopen");
    list_t *L;
    CHECK_EQ(L=createList(), NULL, "createList");

    char line[MAX_LINE];
    while(fgets(line, MAX_LINE, fp) != NULL)
	tokenizeAndInsert(L, line);

    fclose(fp);

    printList(L);
    destroyList(L);
    return 0;
}
