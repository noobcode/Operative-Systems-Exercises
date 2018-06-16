#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>

typedef pthread_mutex_t forchetta_t;
typedef struct thArgs{
	int tid;	     // thread id
	int N;			// numero filosofi
	forchetta_t* forks;	// forchette
}threadArgs;

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

void mangia(unsigned int * seed){
	long r = rand_r(seed);
	struct timespec t = {0, r};
	nanosleep(&t, NULL);
}

void pensa(unsigned int* seed){
	long r = rand_r(seed);
	struct timespec t = {0, r};
	nanosleep(&t, NULL);
}

void* cena(void* arg){
	int myid = ((threadArgs*)arg)->tid;
	int n = ((threadArgs*)arg)->N;
	forchetta_t destra = ((threadArgs*)arg)->forks[myid];
	forchetta_t sinistra = ((threadArgs*)arg)->forks[(myid + 1) % n];
	int i;
	int pensare = 0;
	int mangiare = 0;
	unsigned int seed = myid;
	for(i = 0; i < 10; i++){
		pensa(&seed);
		pensare++;
		
		if(myid % 2 == 0){
			// filosofo di indice pari
			Pthread_mutex_lock(&destra);
			Pthread_mutex_lock(&sinistra);
			mangia(&seed); mangiare++;
			Pthread_mutex_unlock(&sinistra);
			Pthread_mutex_unlock(&destra);
		}else{
			// filosofo di indice dispari
			Pthread_mutex_lock(&sinistra);
			Pthread_mutex_lock(&destra);
			mangia(&seed); mangiare++;
			Pthread_mutex_unlock(&destra);
			Pthread_mutex_unlock(&sinistra);
		}
	
	}
	printf("filosofo%d ho mangiato %d volte e pensato %d volte\n", myid, mangiare, pensare);
	pthread_exit((void*)0);
}

int main(){
	int i, err;
	int n = 4;
	pthread_t* filosofi;
	threadArgs* thArg;
	forchetta_t* forks;
	
	thArg = malloc(n * sizeof(threadArgs));
	if(!thArg){
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	
	forks = malloc(n * sizeof(forchetta_t));
	if(!forks){
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	
	filosofi = malloc(n * sizeof(pthread_t));
	if(!filosofi){
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	
	for(i = 0; i < n; i++){
		thArg[i].tid = i;
		thArg[i].N = n;
		thArg[i].forks = forks;
			
		// inizializzo mutex;
		if(pthread_mutex_init(&forks[i],NULL) != 0){
			fprintf(stderr, "mutex_init failed\n");
			exit(1);
		}
	}
	
	for(i = 0; i < n; i++){	
		// inizializzo thread
		if( (err = pthread_create(&filosofi[i], NULL, &cena, (void*)&thArg[i])) != 0 ){
			fprintf(stderr, "pthread_create fallita\n");
			exit(EXIT_FAILURE);
		}		
	}
	
	for(i = 0; i<n; i++){
		if( (err = pthread_join(filosofi[i], NULL)) != 0 ){
			fprintf(stderr, "pthread_join fallita\n");
			exit(EXIT_FAILURE);
		}
	}	
	free(filosofi);
	free(thArg);
	free(forks);
		
	return 0;
}
