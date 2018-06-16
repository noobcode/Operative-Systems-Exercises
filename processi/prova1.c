#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#define SYSCALL(p,r,s) if((p = r) == -1) { int errnocopy = errno; perror(s); exit(errnocopy); }

#define READ 0
#define WRITE 1

int main() {
	int pipe1[2];
	int pid, status;
	int err; 
	int ppid = getpid(); // pid processo padre
	SYSCALL(err, pipe(pipe1), "pipe");
	SYSCALL(pid, fork(), "fork");
	
	if(pid == 0) { // processo figlio
		SYSCALL(err, dup2(pipe1[READ],0), "dup2");
		SYSCALL(err, close(pipe1[WRITE]), "close");
		SYSCALL(err, close(pipe1[READ]), "close")
		execlp("sort", "sort", (char*) NULL);
		perror("exec");
		exit(errno);	
	} else {	// processo padre
		SYSCALL(err, dup2(pipe1[WRITE],1), "dup2");
		SYSCALL(err, close(pipe1[READ]), "close");
		SYSCALL(err, close(pipe1[WRITE]), "close");
		sleep(1);
		write(1, "z\n", 2);
		write(1, "a\n", 2);
		write(1, "d\n", 2);
		write(1, "c\n", 2);
		write(1, "b\n", 2);
	}
	
	// il processo padre aspetta la fine dell'esecuzione del figlio
	if(getpid() == ppid) {
		SYSCALL(err, waitpid(pid, &status, 0), "waitpid");
		if(WIFEXITED(status))
			printf("processo %d terminato con exit status %d\n", pid, WEXITSTATUS(status));
	}
	return 0;
}
