#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define EOF (-1)
#define LEN 101

int main(int argc, char* argv[]){
	if(argc != 2){
		perror("argc");
		exit(EXIT_FAILURE);
	}
	int count = 0, i = 0;
	char* str, *s;
	
	FILE* in = fopen(argv[1], "r");
	if(!in){
		perror("fopen");
		exit(EXIT_FAILURE);
	}
	FILE* out = fopen("input.log", "w+");
	if(!out){
		perror("fopen");
		exit(EXIT_FAILURE);
	}
	
	while(fgetc(in) != EOF)	count++;
	str = malloc((count+1) * sizeof(char));
	s = malloc(LEN * sizeof(char));
	
	printf("posizione file: %ld\n", ftell(in));
	rewind(in);
	printf("posizione file dopo il rewind: %ld\n", ftell(in));
	
	while(fscanf(in,"%s", s) != EOF){
		strcat(s, " ");
		str = strcat(str,s); // conditional jump or move depends on uninitialized value(s)
	}
	printf("%s\n",str);
	printf("lunghezza : %d\n",strlen(str));
	
	while(str[i] != '\0'){
		fprintf(out,"%c", str[i++]);
	}
	fclose(in);
	fclose(out);
	free(str);
	free(s);
	
	return 0;
}
