// lettura da stdin interrotta

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

// un gestore che ritorna senza terminare il processo
void gestore(int sig){
	write(1, "SIGALRM catturato\n", 18);
}

int main(){
	struct sigaction s;
	char buff[100];
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
	printf("attendo la read...\n");
	if(read(0, buff, 100) == -1){
		perror("read");
	}
	return 0;
}


