#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#define CHECK_LOCK(p,str) if((p) != 0) { errno = p; perror(str); pthread_exit((void*)errno); }
#define N 10
typedef struct node{
	int info;
	struct node* next;
}node;

node* head = NULL;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t itemAdded = PTHREAD_COND_INITIALIZER;
pthread_cond_t itemRemoved = PTHREAD_COND_INITIALIZER;

node* estrai(){
	node* temp = head;
	head = head->next;
	return temp;
}

node* produci(int n){
	node* el = malloc(sizeof(node));
	if(!el){ perror("malloc"); exit(EXIT_FAILURE); }
	el->info = n;
	el->next = NULL;
	return el;
}
void inserisci(node* p) {
	if(!head){
		head = p;
		return;
	}
}

static void* consumer(void* arg){
	node* p;
	while(1){
		CHECK_LOCK(pthread_mutex_lock(&mtx), "lock");
		while(head == NULL){ // while empty
			CHECK_LOCK(pthread_cond_wait(&itemAdded, &mtx), "wait");
			printf("consumer waken up! ");
			fflush(stdout);
		}
		p = estrai();
		CHECK_LOCK(pthread_cond_signal(&itemRemoved), "signal");
		CHECK_LOCK(pthread_mutex_unlock(&mtx), "unlock");
		printf("%d\n", p->info);
		fflush(stdout);
		if(p->info == 9)
			break;
	}
	return (void*)0;
}

static void* producer(void* arg){
	int i;
	node* p;
	for(i = 0; i < N; i++){
		p = produci(i);
		CHECK_LOCK(pthread_mutex_lock(&mtx), "lock");
		while(head){ // while full
			CHECK_LOCK(pthread_cond_wait(&itemRemoved, &mtx), "wait");
		}		
		inserisci(p);
		CHECK_LOCK(pthread_cond_signal(&itemAdded), "signal");
		CHECK_LOCK(pthread_mutex_unlock(&mtx), "unlock");
	}
	return (void*)0;
}

int main(){
	int err;
	pthread_t t1, t2, status1, status2;
	node* p;
	err = pthread_create(&t1, NULL, &producer, NULL);
	CHECK_LOCK(err, "pthread_create1");
	err = pthread_create(&t2, NULL, &consumer, NULL);
	CHECK_LOCK(err, "pthread_create2");
	sleep(5);
	CHECK_LOCK(pthread_join(t1, (void*) &status1), "join1");
	CHECK_LOCK(pthread_join(t2, (void*) &status2), "join2");
	return 0;
}
