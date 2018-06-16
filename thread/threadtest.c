#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static int x;

void Pthread_mutex_lock(pthread_mutex_t* mtx){
	int err;
	if( (err = pthread_mutex_lock(mtx)) != 0){
		errno = err;
		perror("lock");
		pthread_exit((void *)errno);
	} else {
		printf("locked ");	
	}
}
void Pthread_mutex_unlock(pthread_mutex_t* mtx){
	int err;
	if( (err = pthread_mutex_unlock(mtx)) != 0){
		errno = err;
		perror("unlock");
		pthread_exit((void*)errno);
	} else {
		printf("unlocked ");	
	}
}

static void* myfun(void* arg){
	int done;
	while(1){
		Pthread_mutex_lock(&mtx);
		done = (x >= 5);
		if(!done) 
			printf("secondo thread: x = %d\n", ++x);
		Pthread_mutex_unlock(&mtx);
		if(done)
			break; 
		sleep(1);
	}
}

int main(){
	pthread_t tid;
	int err, done;
	
	if( (err = pthread_create(&tid, NULL, &myfun, NULL)) != 0){
		// gestione errore
	} else {
		// secondo thread creato
		while(1){
			Pthread_mutex_lock(&mtx);
			done = (x >= 5);
			if(!done) 
				printf("primo thread: x = %d\n", ++x);
			Pthread_mutex_unlock(&mtx);
			if(done)
				break;
			sleep(1);
		}
	}
	
	return 0;
}
