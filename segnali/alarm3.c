#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

volatile sig_atomic_t sigalarm_flag = 0;

void gestore(int sig){
	sigalarm_flag = 1;
}

int main(){
	struct sigaction s;
	if(sigaction(SIGALRM, NULL, &s) == -1){ 
		perror("sigaction");
		exit(1);
	}
	s.sa_handler = gestore;
	if(sigaction(SIGALRM, &s, NULL) == -1){ 
		perror("sigaction");
		exit(1);
	}
	alarm(3);
	printf("ciclo infinito\n");
	while(sigalarm_flag != 1)
		pause(); // ciclo fino a SIGALRM
		// serve a mettersi in pausa solo se sigalarm non è ancora arrivato
	printf("pippo\n");
	return 0;
}

