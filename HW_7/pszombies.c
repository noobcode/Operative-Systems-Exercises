/* Esercizio 3:
 * 
 * Esegue l'equivalente del seguente comando di shell:
 * ps -A -ostat,ppid | grep -e '[zZ]'| awk '{ print $2 }'
 *
 * Il processo main lancia tre processi, uno per ogni comando della pipe.
 *
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#include <util.h>

int main() {
    int r;    
    int pipe1[2];

    SYSCALL(pipe, r, pipe(pipe1), "pipe %d", 1);

    int pid;
    SYSCALL(fork,pid,fork(),"fork%d",1);
    if (pid == 0) {
		dup2(pipe1[1], 1);
		close(pipe1[0]);
		execlp("ps", "ps", "-A", "-ostat,ppid",NULL);
		perror("execlp1");
		exit(errno);
    } else {
		dup2(pipe1[0],0);
		close(pipe1[1]);
		int pipe2[2];
		SYSCALL(pipe, r, pipe(pipe2), "pipe%d", 2);
		SYSCALL(fork, pid,fork(),"fork%d",1);
		if (pid == 0) {
	    	dup2(pipe2[1],1);
	    	close(pipe2[0]);
	    	execlp("grep", "grep", "-e", "[zZ]", NULL);
	    	perror("execl2");
	    	exit(errno);
		} else {
	    	dup2(pipe2[0],0);
	    	close(pipe2[1]);
			SYSCALL(fork, pid,fork(),"fork%d", 1);
	    	if (pid == 0) {
				execlp("awk", "awk", "\"{ print $2 }\"", NULL);
				perror("execl3");
				exit(errno);
	    	}
		}	
    }

    int status;
    SYSCALL(wait, r, wait(&status), "wait pid=%d",r);
    CHECK_WAIT(r,status,stderr);
    SYSCALL(wait, r, wait(&status), "wait pid=%d", r);
    CHECK_WAIT(r,status,stderr);
    SYSCALL(wait, r, wait(&status), "wait pid=%d", r);
    CHECK_WAIT(r,status,stderr);
    return 0;
}
