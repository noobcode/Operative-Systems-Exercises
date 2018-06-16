#define _GNU_SOURCE 1
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <assert.h>
#include <signal.h>
#include <queue.h>

typedef struct threadArgs {
    int      thid;
    Queue_t *q;
    int      start;
    int      stop;
} threadArgs_t;

typedef struct sigHandlerArgs {
    Queue_t  *q;
    sigset_t *set;
} sigHandlerArgs_t;

void data2String(char *buf, size_t s, void *data) {
    snprintf(buf, s, "%d", *(int*)data);
}

void *sigHandler(void *arg) {
    Queue_t *q    = ((sigHandlerArgs_t*)arg)->q;
    sigset_t *set = ((sigHandlerArgs_t*)arg)->set;
    int sig;

    for( ;; ) {
	int r = sigwait(set, &sig);
	if (r != 0) {
	    errno = r;
	    perror("sigwait");
	    pthread_exit(NULL);
	}
	queueStatus(q, data2String);
    }

    return NULL;	   
}

void *Producer(void *arg) {
    Queue_t *q  = ((threadArgs_t*)arg)->q;
    int   myid  = ((threadArgs_t*)arg)->thid;
    int   start = ((threadArgs_t*)arg)->start;
    int   stop  = ((threadArgs_t*)arg)->stop;
    for(int i=start;i<stop; ++i) {
	int *data = malloc(sizeof(int));
	if (data == NULL) {
	    perror("Producer malloc");
	    pthread_exit(NULL);
	}
	*data = i;
	if (push(q, data) == -1) {
	    fprintf(stderr, "Errore: push\n");
	    pthread_exit(NULL);
	}
	printf("producer%d pushed %d\n", myid, i);
    }
    printf("Producer exits\n");
    pthread_exit(NULL);
}

void *Consumer(void *arg) {
    Queue_t *q  = ((threadArgs_t*)arg)->q;
    int   myid  = ((threadArgs_t*)arg)->thid;

    while(1) {
	int *data;
	data = pop(q);
	assert(data);
	if (*data == -1) { free(data); break;}
	printf("consumer%d: estratto %d\n", myid, *data);
	free(data);
    }

    printf("Consumer exits\n");
    pthread_exit(NULL);
}


void usage(char *pname) {
    fprintf(stderr, "\nusa: %s -p <num-producers> -c <num-consumers> -n <num-messages>\n\n", pname);
    exit(EXIT_FAILURE);
}
	    

int main(int argc, char *argv[]) { 
    extern char *optarg;
    int p=0,c=0, n=0, opt;    
    while((opt = getopt(argc, argv, "p:c:n:")) != -1) {
	switch(opt) {
	case 'p':
	    p=atoi(optarg);
	    break;
	case 'c':
	    c=atoi(optarg);
	    break;
	case 'n':
	    n=atoi(optarg);
	    break;
	default:
	    usage(argv[0]);
	    break;
	}
    }
    if (p==0 || c==0 || n==0) 	usage(argv[0]);
    printf("num producers =%d, num consumers =%d\n", p, c);

    sigset_t set;
    sigemptyset(&set);        // resetto tutti i bits
    sigaddset(&set, SIGUSR1); // aggiunto SIGUSR1 alla machera
    // blocco SIGUSR1 -- NOTA: la maschera viene ereditata da tutti i threads
    if (pthread_sigmask(SIG_BLOCK, &set, NULL) != 0) {
	fprintf(stderr, "ERROR: pthread_sigmask\n");
	return (EXIT_FAILURE);
    }

    pthread_t    *th, sigth;
    threadArgs_t *thARGS;

    th     = malloc((p+c)*sizeof(pthread_t));
    thARGS = malloc((p+c)*sizeof(threadArgs_t));
    if (!th || !thARGS) {
	fprintf(stderr, "malloc fallita\n");
	exit(EXIT_FAILURE);
    }
    
    Queue_t *q = initQueue();
    if (!q) {
	fprintf(stderr, "initQueue fallita\n");
	exit(errno);
    }
    
    int chunk = n/p, r= n%p;
    int start = 0;
    for(int i=0;i<(p+c); ++i) {	
	thARGS[i].thid = i;
	thARGS[i].q    = q;
	thARGS[i].start= start;
	thARGS[i].stop = start+chunk + ((i<r)?1:0);
	start = thARGS[i].stop;	
    }

    // creo il sighandler thread passando come argomenti il puntatore alla
    // coda e la signal mask
    sigHandlerArgs_t sigArgs = { q, &set };
    if (pthread_create(&sigth, NULL /*&thattr*/, sigHandler, &sigArgs) != 0) {
	fprintf(stderr, "pthread_create failed (sigHandler)\n");
	exit(EXIT_FAILURE);
    }
        
    for(int i=0;i<c; ++i)
	if (pthread_create(&th[p+i], NULL, Consumer, &thARGS[p+i]) != 0) {
	    fprintf(stderr, "pthread_create failed (Consumer)\n");
	    exit(EXIT_FAILURE);
	}
    for(int i=0;i<p; ++i)
	if (pthread_create(&th[i], NULL, Producer, &thARGS[i]) != 0) {
	    fprintf(stderr, "pthread_create failed (Producer)\n");
	    exit(EXIT_FAILURE);
	}

    /* possibile protocollo di terminazione:
     * si aspettano prima tutti i produttori
     * quindi si inviano 'c' end-of-stream (-1)
     * quindi si aspettano i consumatori
     */
    // aspetto prima tutti i produttori
    for(int i=0;i<p; ++i)
	pthread_join(th[i], NULL);
    // quindi termino tutti i consumatori
    for(int i=0;i<c; ++i) {
	int *eos = malloc(sizeof(int));
	*eos = -1;
	push(q, eos);
    }
    // aspetto la terminazione di tutti i consumatori
    for(int i=0;i<c; ++i)
	pthread_join(th[p+i], NULL);

    // termino il thread sigHandler (sigwait e' un cancellation point)  
    if (pthread_cancel(sigth) != 0) {
	fprintf(stderr, "pthread_cancel failed\n");
	return EXIT_FAILURE;
    }
    // ed aspetto la sua uscita
    pthread_join(sigth, NULL);

    // libero memoria
    deleteQueue(q);
    free(th);
    free(thARGS);
    return 0;   
}
