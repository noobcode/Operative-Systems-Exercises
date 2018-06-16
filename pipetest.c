#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
 

int main(){
	int pfd[2];
	long int v;
	// creazione pipe
	if(pipe(pfd) == -1){
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	// return the size of the pipe buffer
	printf("POSIX = %ld e ", _POSIX_PIPE_BUF);
	errno = 0;
	if((v = fpathconf(pfd[0], _PC_PIPE_BUF)) == -1){
		if(errno != 0){
			perror("fpathconf");
			exit(EXIT_FAILURE);
		}else
			printf("reale = illimitato\n");
	}else
		printf("reale = %ld\n", v);
	return 0;	
}
