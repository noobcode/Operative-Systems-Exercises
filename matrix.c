#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#define N 4

void checkFailure(void* p, char* s){
	if(!p){
		perror(s);
		exit(EXIT_FAILURE);
	}
}

void cleanup(float** m){
	int i;
	for(i = 0; i < N; i++)
		free(m[i]);
	free(m);
}

void stampa(float **m){
	int i,j;
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++)
			printf("%.1f ", m[i][j]);
		printf("\n");
	}		
}

int main(){
	int i,j;
	float** m,**m2;
	
	FILE* f = fopen("./mat_dump.dat","wb");
	checkFailure(f,"fopen");
	
	m = malloc(N * sizeof(float*)); //alloca m
	checkFailure(m,"malloc");
	for(i = 0; i < N; i++){
		m[i] = malloc(N * sizeof(float));
		checkFailure(m[i], "malloc");}
	
	for(i = 0; i < N; i++)	// riempi m
		for(j = 0; j < N; j++)
			m[i][j] = (i+j)/2.0;
			
	for(i = 0; i < N; i++)	// scrivi su file
		for(j = 0; j < N; j++){
			if(fwrite(&m[i][j], sizeof(float), 1, f) != 1){
				perror("fwrite");
				exit(EXIT_FAILURE);
			}
		}
	fclose(f);	// chiudo lo stream in scrittura
	f = fopen("./mat_dump.dat", "rb"); // riapro stream in lettura
	checkFailure(f, "fopen");
	
	m2 = malloc(N * sizeof(float*));
	checkFailure(m,"malloc");	
	for(i = 0; i < N; i++){
		m2[i] = malloc(N * sizeof(float));
		checkFailure(m2[i], "malloc");}	
	for(i = 0; i < N; i++)
		for(j = 0; j < N; j++){
			if(fread(&m2[i][j], sizeof(float), 1, f) != 1){
				perror("fread");
				exit(EXIT_FAILURE);
			}
		}
				
	stampa(m);	printf("\n");
	stampa(m2);
	
	printf("memcmp : %d\n", memcmp(m,m2,(N*N) * sizeof(float)));		
	if (memcmp(m, m2, N*N*sizeof(float)) != 0) {
		cleanup(m);
		cleanup(m2);
		fclose(f);	
		fprintf(stderr, "Le due matrici non corrispondono\n");
	return EXIT_FAILURE;
    }
    fprintf(stdout, "Le due matrici sono uguali\n");
	cleanup(m);
	cleanup(m2);
	fclose(f);		
	return 0;
}
