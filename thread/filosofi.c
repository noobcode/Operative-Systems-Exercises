#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>
#define N 4

typedef enum{ FAME, MANGIA, PENSA }stato_t;

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t attesaFilosofo[N];
stato_t stato[N];

void Pthread_mutex_lock(pthread_mutex_t* mtx){
	int err;
	if( (err = pthread_mutex_lock(mtx)) != 0){
		errno = err;
		perror("lock");
		pthread_exit((void *)errno);
	}
}

void Pthread_mutex_unlock(pthread_mutex_t* mtx){
	int err;
	if( (err = pthread_mutex_unlock(mtx)) != 0){
		errno = err;
		perror("unlock");
		pthread_exit((void*)errno);
	}
}

int sx(int i, int dec){
	if(i < 2)
		return (i-dec) + N;
	else
		return (i-dec);
}

void mangia(unsigned int * seed, int i){
	long r = rand_r(seed);
	struct timespec t = {0, r};
	printf("filosofo %d: mangia\n", i);
	fflush(stdout);
	nanosleep(&t, NULL);
}
void pensa(unsigned int* seed,int i){
	long r = rand_r(seed);
	struct timespec t = {0, r};
	printf("filosofo %d: pensa\n", i);
	fflush(stdout);
	nanosleep(&t, NULL);
}

static void* cena(void* arg){
	int i = (int) arg;
	int j;
	unsigned int seed = i;
	
	for(j = 0; j < 2; j++){
		// protocollo per mangiare
		Pthread_mutex_lock(&mtx);
		stato[i] = FAME;
		printf("filosofo %d: fame\n", i);
		fflush(stdout);
		
		while(stato[sx(i, 1)] == MANGIA || stato[(i+1) % N] == MANGIA)
			pthread_cond_wait(&attesaFilosofo[i], &mtx);
	
		// ha ottenuto tutti e due i bastoncini
		stato[i] = MANGIA;
		mangia(&seed, i);
		Pthread_mutex_unlock(&mtx);
		
		
		// il filosofo i ha finito di mangiare
		// protocollo per pensare
		Pthread_mutex_lock(&mtx);
		stato[i] = PENSA;
		
		if(stato[sx(i,1)] == FAME && stato[sx(i,2)] != MANGIA){
			// riattiva il filosofo i-1 se può ottenere entrambi i bastoncini
			stato[sx(i,1)] = MANGIA;
			pthread_cond_signal(&attesaFilosofo[sx(i,1)]);
		}
		if(stato[(i+1) % N] == FAME && stato[(i+2) % N] != MANGIA){
			// riattiva il filosofo i+1 se può ottenere entrambi i bastoncini
			stato[(i+1) % N] = MANGIA;
			pthread_cond_signal(&attesaFilosofo[(i+1) % N]);
		}
		Pthread_mutex_unlock(&mtx);
		pensa(&seed, i);
	}
	pthread_exit((void*)0);
}

int main(){
	int i, err, status;
	pthread_t filosofo[N];
	
	for(i = 0; i < N; i++)
		pthread_cond_init(&attesaFilosofo[i], NULL);
	
	for(i = 0; i < N; i++)
		if( (err = pthread_create(&filosofo[i], NULL, &cena, (void*)i)) != 0 ){
			perror("pthread_create");
			exit(err);
		}
	
	for(i = 0; i < N; i++){
		if ( pthread_join(filosofo[i], (void*) &status) == -1 )
			fprintf(stderr,"errore pthread_join\n");
		printf("filosofo %d terminato con exit(%d)\n",i, status);
	}
		
	return 0;	
}
