#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

int main(){
	sigset_t set;
	int sig;
	
	// costruisco la maschera con solo SIGALRM
	sigemptyset(&set);
	sigaddset(&set, SIGALRM);
	
	// blocco SIGALRM
	pthread_sigmask(SIG_SETMASK, &set, NULL);
	alarm(3); // SIGALRM fra 3 secs
	printf("inizio attesa...\n");
	sigwait(&set, &sig);
	printf("pippo, %d\n", sig);
	return 0;

}

