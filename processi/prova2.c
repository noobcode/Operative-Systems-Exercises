#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#define READ 0
#define WRITE 1
#define BUFFSIZE 20 
#define SYSCALL(p,s) if((p) == -1){ perror(s); exit(errno); }

int main(){
	int pipefd[2];
	pid_t pid;
	char buff[BUFFSIZE];
	
	SYSCALL(pipe(pipefd), "pipe");
	SYSCALL(pid = fork(), "fork");
	
	if(pid){
		// parent process
		SYSCALL(close(pipefd[READ]), "close");
		SYSCALL(write(pipefd[WRITE], "messaggio prova", BUFFSIZE), "write");
		SYSCALL(close(pipefd[WRITE]), "close");
	} else {
		// child process
		SYSCALL(close(pipefd[WRITE]), "close");
		SYSCALL(read(pipefd[READ], buff, BUFFSIZE), "read");
		SYSCALL(close(pipefd[READ]), "close");
		printf("buffer = %s\n", buff);
	}
		
	return 0;
}
