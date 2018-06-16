#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int comparestring(const void* a,const void* b){
	return strcmp(*(char**)a, *(char**)b);
}

int cmpstring(const void *str1, const void *str2)  {
    const size_t str1len = strlen(*(char**)str1);
    const size_t str2len = strlen(*(char**)str2);

    return strncmp(*(char**)str1, *(char**)str2, ((str1len <= str2len)? str1len : str2len));
}

int main(int argc, char* argv[]){
	int i;
	for(i = 0; i < argc; ++i) 
		printf("argv[%d]= %s\n", i, argv[i]);
	qsort(argv, argc, sizeof(char*), cmpstring);
	printf("\n");
	for(i = 0; i < argc; ++i) 
		printf("argv[%d]= %s\n", i, argv[i]);
	return 0;
}
