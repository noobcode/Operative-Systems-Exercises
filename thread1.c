#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#define CHECK_LOCK(p,str) if((p) != 0) { errno = p; perror(str); pthread_exit((void*)errno); }

static int x = 0; // variabile condivisa
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

static void* myfun(void* arg){
	int err;
	while(x < (int)arg){
		CHECK_LOCK(err = pthread_mutex_lock(&mtx), "lock");
		printf("locked ");
		
		printf("secondo thread: x = %d\n",++x);
		
		CHECK_LOCK(err = pthread_mutex_unlock(&mtx), "unlock");
		printf("unlocked ");
		sleep(1);
	}
	// equivalente a 'return (void*)17;'
	pthread_exit((void*)17);
}

int main(){
	pthread_t tid;
	int err, status; //per l'exit status
	
	// controllo prima che lo spazio sia sufficiente per rappresentare un intero.
	assert(sizeof(int) <= sizeof(void*));
	
	if((err = pthread_create(&tid, NULL, &myfun, (void*)10)) != 0){
		perror("pthread_create");
		exit(err);
	}else{
		while(x < 10){
			CHECK_LOCK(err = pthread_mutex_lock(&mtx), "lock");
			printf("locked ");
			
			printf("primo thread: x = %d\n", ++x);
			
			CHECK_LOCK(err = pthread_mutex_unlock(&mtx), "unlock");
			printf("unlocked ");
			sleep(1);
		}
		pthread_join(tid, (void*)&status);
		printf("thread 2 terminato: %d status\n", status);
	}
	return 0;
}
