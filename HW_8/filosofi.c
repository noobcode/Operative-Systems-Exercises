/*
 * Esercizio 2:
 *
 */
#define _GNU_SOURCE 1
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
//#include <queue.h>

typedef pthread_mutex_t forchetta_t;

typedef struct threadArgs {
    int          thid;
    int          N;
    forchetta_t *forks;
} threadArgs_t;

void Mangia(unsigned int *seed) {
    long r = rand_r(seed) % 800000;
    // per perdere un po' di tempo si puo'
    // usare anche il seguente ciclo for
    // for(volatile long i=0;i<r; ++i);
    struct timespec t={0, r};
    nanosleep(&t,NULL);
}
void Pensa(unsigned int *seed) {
    long r = rand_r(seed) % 1000000;
    //for(volatile long i=0;i<r; ++i);
    struct timespec t={0, r};
    nanosleep(&t,NULL);
}

// thread filosofo
void *Filosofo(void *arg) {
    int   myid  = ((threadArgs_t*)arg)->thid;
    int   N     = ((threadArgs_t*)arg)->N;
    forchetta_t destra   = ((threadArgs_t*)arg)->forks[myid];
    forchetta_t sinistra = ((threadArgs_t*)arg)->forks[(myid+1) % N];
    unsigned int seed = myid;
    int mangia = 0;
    int pensa  = 0;

    for(int i=0;i<100; ++i) {
	Pensa(&seed); ++pensa;

	// fisso un ordinamento totale tra i filosofi per l'acquisizione delle forchette
	if (myid % 2) { // filosofo di indice dispari
	    pthread_mutex_lock(&destra);
	    pthread_mutex_lock(&sinistra);
	    ++mangia;
	    Mangia(&seed);
	    pthread_mutex_unlock(&sinistra);
	    pthread_mutex_unlock(&destra);
	    
	} else {  // filosofo di indice pari
	    pthread_mutex_lock(&sinistra);
	    pthread_mutex_lock(&destra);
	    ++mangia;
	    Mangia(&seed);
	    pthread_mutex_unlock(&destra);
	    pthread_mutex_unlock(&sinistra);
	}
    }
    printf("Filosofo%d ho mangiato %d volte e pensato %d volte\n", myid, mangia, pensa);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) { 
    int N = 5;
    if (argc > 1) {
	N = atoi(argv[1]);
	if (N > 20) {
	    fprintf(stderr, "N ridotto a 20\n");
	    N=20;
	}
    }
    pthread_t    *th;
    threadArgs_t *thARGS;
    forchetta_t  *forks;
    th     = malloc(N*sizeof(pthread_t));
    thARGS = malloc(N*sizeof(threadArgs_t));    
    // array di mutex una per ogni forchetta
    forks  = malloc(N*sizeof(forchetta_t));
    if (!th || !thARGS || !forks) {
	fprintf(stderr, "malloc fallita\n");
	exit(EXIT_FAILURE);
    }   
    for(int i=0;i<N; ++i) {	
	thARGS[i].thid  = i;
	thARGS[i].N     = N;
	thARGS[i].forks = forks;
	if (pthread_mutex_init(&forks[i], NULL) != 0) {
	    fprintf(stderr, "pthread_mutex_init fallita\n");
	    exit(EXIT_FAILURE);
	}
    }
    for(int i=0; i<N; ++i)
	if (pthread_create(&th[i], NULL, Filosofo, &thARGS[i]) != 0) {
	    fprintf(stderr, "pthread_create failed\n");
	    exit(EXIT_FAILURE);
	}

    for(int i=0;i<N; ++i)
	if (pthread_join(th[i], NULL) == -1) {
	    fprintf(stderr, "pthread_join failed\n");
	}

    return 0;   
}
