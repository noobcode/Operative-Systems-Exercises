#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

// gestore segnale
static void gestore(int signum){
	printf("ricevuto segnale %d\n", signum);
	exit(EXIT_FAILURE);
}

int main(){
	struct sigaction s;
	int i;
	
	// inizializzo s a 0
	memset(&s, 0, sizeof(s));
	
	// registro gestore
	//s.sa_handler = gestore;
	
	// ignoro SIGINT
	s.sa_handler = SIG_IGN;
	
	// installo nuovo gestore s
	if(sigaction(SIGINT, &s, NULL) == -1){
		perror("sigaction");
		exit(1);
	}	
	
	for(i = 0; ; i++){
		sleep(1);
		printf("%d\n", i);
	}
	exit(EXIT_SUCCESS);
	
}
