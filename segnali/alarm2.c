#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

// per avere un vero timer dobbiamo personalizzare SIGALARM
void gestore(int sig){
	write(1, "SIGALARM catturato\n", 18);
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
	alarm(3); // SIGALRM fra 3 secs
	while(1);
	printf("pippo\n"); // mai eseguita
	return 0;	
}
