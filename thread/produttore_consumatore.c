#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

typedef struct node{
	int data;
}node;

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
node* head = NULL;


void Pthread_mutex_lock(pthread_mutex_t* mtx){
	int err;
	if( (err = pthread_mutex_lock(mtx)) != 0){
		errno = err;
		perror("lock");
		pthread_exit((void *)errno);
	} else {
		printf("locked\n");	
	}
}

void Pthread_mutex_unlock(pthread_mutex_t* mtx){
	int err;
	if( (err = pthread_mutex_unlock(mtx)) != 0){
		errno = err;
		perror("unlock");
		pthread_exit((void*)errno);
	} else {
		printf("unlocked\n");	
	}
}

node* produci(int i){
	node* new = malloc(sizeof(node));
	new->data = i;
	return new;
}

void inserisci(node* item){
	if(!head)
		head = item;
	return;
}

node* estrai(){
	node* item;
	if(head){
		item = head;
		head = NULL;
		return item;
	}
}

void* producer(void* arg){
	int n, i;
	node* item;
	n = (int)arg;
	for(i = 0; i < n; i++){
		item = produci(i);
		Pthread_mutex_lock(&mtx);
		while(head){
			pthread_cond_wait(&empty, &mtx);
		}
		inserisci(item);
		pthread_cond_signal(&full);
		Pthread_mutex_unlock(&mtx);
	}
	return (void*) 0;
}

void* consumer(void* arg){
	node* item;
	while(1){
		Pthread_mutex_lock(&mtx);
		while(!head){
			pthread_cond_wait(&full, &mtx);
		}
		item = estrai();
		pthread_cond_signal(&empty);
		Pthread_mutex_unlock(&mtx);
		printf("consumato %d\n", item->data);
		fflush(stdout);
		if(item->data == 2)
			break;
	}
	return (void*) 0;
}

int main(){
	pthread_t prod, cons;
	int status1, status2;
	int err;
	
	if( (err = pthread_create(&prod, NULL, &producer, (void* )3)) != 0 ){
		// gestione errore
		perror("pthread_create 1");
		exit(err);
	} else if( (err = pthread_create(&cons, NULL, &consumer, NULL)) != 0 ) {
		// gestione errore
		perror("pthread_create 2");
		exit(err);
	}	
	
	pthread_join(prod, (void*) &status1);
	pthread_join(cons, (void*) &status2);
	printf("thread producer terminato con exit status %d\n", status1);
	printf("thread consumer terminato con exit status %d\n", status2);	
	
	return 0;	
}
