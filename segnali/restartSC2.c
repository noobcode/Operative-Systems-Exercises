// lettura da stdin per evitare l'interruzione

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

// un gestore che ritorna senza terminare il processo
void gestore(int sig){
	write(1, "SIGALRM catturato\n", 18);
}

int main(){
	struct sigaction s;
	char buff[100];
	int l;
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
	
	// EINTR : the call was interrupted by a signal before any data was read
	while( (l = read(0, buff, 100)) == -1 && (errno = EINTR) );
	if(l < 0)
		perror("read");
	else
		write(1, buff, l);
	return 0;
}
