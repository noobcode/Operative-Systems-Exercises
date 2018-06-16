#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define LEN 101

void scrivi(FILE* f){
	char* str = malloc(LEN * sizeof(char));
	int i;
	for(i = 0; i < 3; i++){
		scanf("%s",str);
		fprintf(f,"%s\n",str);
	}
	free(str);	
}

void stampa(FILE* f){
	char* str = malloc(LEN * sizeof(char));
	while(fscanf(f, "%s", str) != EOF){
		fprintf(stderr, "%s\n", str);
	}
	free(str);
}

int main(){
	FILE* f1, *f2;
	f1 = fopen("file1", "w+");
	f2 = fopen("file2", "w+");
	
	scrivi(f1);
	scrivi(f2);
	
	fseek(f1, 0, SEEK_SET);
	rewind(f2);
	
	stampa(f1);
	stampa(f2);
	
	fclose(f1);
	fclose(f2);
	
	return 0;
}
