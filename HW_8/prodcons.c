/*
 * Esercizio 1
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;
static int buffer;       // risorsa condivisa
static char bufempty=1;  // flag

// produttore
void *Producer(void *notused) {
    for(int i=0;i<100;++i) {
	pthread_mutex_lock(&mutex);
	while(!bufempty)
	    pthread_cond_wait(&cond,&mutex);
	
	buffer = i;
	bufempty = 0;
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mutex);
    }
    printf("Producer exits\n");
    pthread_exit(NULL);
}

// consumatore 
void *Consumer(void *notused) {
    for(int i=0;i<100;++i) {
	pthread_mutex_lock(&mutex);
	while(bufempty) 
	    pthread_cond_wait(&cond,&mutex);

	int val = buffer;
	bufempty = 1;
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mutex);

	printf("Consumer got:   %d\n", val);
    }
    printf("Consumer exits\n");
    pthread_exit(NULL);
}

int main() {
    pthread_t thconsumer, thproducer;
    if (pthread_create(&thconsumer, NULL, Consumer, NULL) != 0) {
	fprintf(stderr, "pthread_create failed (Consumer)\n");
	return (EXIT_FAILURE);
    }
    if (pthread_create(&thproducer, NULL, Producer, NULL) != 0) {
	fprintf(stderr, "pthread_create failed (Producer)\n");
	return (EXIT_FAILURE);
    }    
    if (pthread_join(thproducer,NULL) != 0) {
	fprintf(stderr, "pthread_join failed (Producer)\n");
	return (EXIT_FAILURE);
    }
    if (pthread_join(thconsumer,NULL) != 0) {
	fprintf(stderr, "pthread_join failed (Consumer)\n");
	return (EXIT_FAILURE);
    }
    return 0;
}
