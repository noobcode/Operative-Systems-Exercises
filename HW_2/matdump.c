/*
 * ASSEGNAMENTO2-2:
 * Scrivere un programma che prende in ingresso un intero (<=512), alloca in memoria una matrice
 * di 512x512 elementi float contigui in memoria. Inizializzare tale matrice (M) in modo arbitrario 
 * (es. M(i,j) = (i+j)/2.0). 
 * Fare quindi il dump della matrice in formato binario su un file il cui nome è 'mat_dump.dat'.
 * Rileggere quindi la matrice dal file (mat_dump.dat) e memorizzarla in memoria in una matrice differente 
 * da quella di partenza (M2) ed effettuare il confronto delle due matrici con la funzione di libreria 'memcmp'.
 *
 * COMPILAZIONE:
 *  gcc -std=c99 -Wall -Werror -I. -o matdump matdump.c 
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DIM_LIMIT 512

// macro di utilità per controllare errori in chiamate di libreria
#define CHECK_CALL_PTR(X, str)	\
    if ((X)==NULL) {		\
      perror(#str);		\
      exit(EXIT_FAILURE);	\
    }
void stampa(float *m, long dim){
	int i,j;
	for(i = 0; i < dim; i++){
		for(j = 0; j < dim; j++)
			printf("%.1f ", m[i*dim+j]);
		printf("\n");
	}    
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
	fprintf(stderr, "use: %s <dim>\n", argv[0]);
	return EXIT_FAILURE;
    }
    long dim = strtol(argv[1], NULL, 10);
    if (dim > DIM_LIMIT) {
	fprintf(stderr,"dim (%ld) è troppo grande, deve essere <=%d\n", dim, DIM_LIMIT);
	return EXIT_FAILURE;
    }

    float *mat = NULL;
    // alloco la matrice come un singolo vettore
    CHECK_CALL_PTR(mat = malloc(dim*dim*sizeof(float)), "malloc");
    
    for(long i=0;i<dim;++i)
	for(long j=0;j<dim;++j)
	    mat[i*dim + j] = (i+j)/2.0;

    FILE *fp;
    CHECK_CALL_PTR(fp = fopen("./mat_dump.dat", "wb"), "fopen");

#if (1 == 0) // write singola
    if (fwrite(&mat[0], sizeof(float), dim*dim, fp) != (dim*dim)) {
	perror("fwrite");
	return EXIT_FAILURE;
    }
#else // alternativamente 
    for(long i=0;i<dim;++i)
	for(long j=0;j<dim;++j)
	    if (fwrite(&mat[i*dim+j], sizeof(float), 1, fp) != 1) {
		perror("fwrite");
		return EXIT_FAILURE;
	    }
#endif
    fclose(fp);

    float *mat2 = NULL;
    CHECK_CALL_PTR(mat2 = malloc(dim*dim*sizeof(float)), "malloc");
    CHECK_CALL_PTR(fp = fopen("./mat_dump.dat", "rb"), "fopen");

#if (1==0) // read singola
    if (fread(&mat2[0], sizeof(float), dim*dim, fp) != (dim*dim)) {
	perror("fread");
	return EXIT_FAILURE;
    }
#else // alternativamente
    for(long i=0;i<dim;++i)
	for(long j=0;j<dim;++j)
	    if (fread(&mat2[i*dim+j], sizeof(float), 1, fp) != 1) {
		perror("fread");
		return EXIT_FAILURE;
	    }
#endif
    fclose(fp);
	stampa(mat2,dim);
    if (memcmp(mat, mat2, dim*dim*sizeof(float)) != 0) {
	fprintf(stderr, "Le due matrici non corrispondono\n");
	return EXIT_FAILURE;
    }
    fprintf(stdout, "Le due matrici sono uguali\n");
    free(mat);
    free(mat2);
    return 0;

}




