#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>



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
	int done, retval;
	while(1){
		retval = incr_x(1);
		if(retval)
			printf("secondo thread: x = %d\n", retval);
		else
			break;
		sleep(1);
	}
}

int incr_x(int incr){
	int retval, done;
	static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
	static int x;
	
	Pthread_mutex_lock(&mtx);
	done = (x >= 5);
	if(!done)
		retval = (x += incr);
	else
		retval = 0;
	Pthread_mutex_unlock(&mtx);
	
	return retval;
}

int main(){
	pthread_t tid;
	int err, done;
	int retval;
	
	if( (err = pthread_create(&tid, NULL, &myfun, NULL)) != 0){
		// gestione errore
	} else {
		// secondo thread creato
		while(1){
			retval = incr_x(1);
			if(retval)
				printf("primo thread: x = %d\n", retval);
			else
				break;
			sleep(1);
		}
	}
	
	return 0;
}
