#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#define CHECK_1(p,str) if(p == -1){ int e = errno; perror(str); exit(e); }

int main(){
	int pid1,pid2,pid3;
	int pipefd1[2], pipefd2[2];
	int err;
	
	// pipe 1
	err = pipe(pipefd1);	CHECK_1(err, "pipe");
	pid1 = fork();	CHECK_1(pid1, "fork");
	if(pid1 == 0){
		// processo figlio 1
		// 'who' scrive sullo stdout. "invece scrivilo in pipefd[1]"
		err = dup2(pipefd1[1],1);	CHECK_1(err, "dup1");
		err = close(pipefd1[0]);	CHECK_1(err, "close1");
		// esegui comando 'who'
		execlp("who", "who", (char*)NULL);
		perror("execlp");
		exit(errno);
	}else{
		// processo padre
		// "invece di prendere l'input dallo stdin, prendilo da pipefd1[0]"
		err = dup2(pipefd1[0],0);	CHECK_1(err, "dup");
		err = close(pipefd1[1]);	CHECK_1(err, "close2");
		
		err = pipe(pipefd2);	CHECK_1(err, "pipe");
		pid2 = fork();	CHECK_1(pid2, "fork");
		if(pid2 == 0){
			// processo figlio 2
			// 'sort' scrive sullo stdout, "invece scrivilo nella pipefd2[1]"
			err = dup2(pipefd2[1],1);	CHECK_1(err, "dup2");
			err = close(pipefd2[0]);	CHECK_1(err, "close3");
			// esegui comando 'sort'
			execlp("sort", "sort", (char*)NULL);
			perror("execlp");
			exit(errno);
		}else{
			// processo padre
			// a questo punto lo 0 corrisponde a pipefd1[0], lo redirigo su pipefd2[0]
			err = dup2(pipefd2[0], 0);
			err = close(pipefd2[1]);	CHECK_1(err, "close4");
			pid3 = fork();	CHECK_1(pid3, "fork");
			if(pid3 == 0){
				// processo figlio 3
				err = dup2(pipefd2[0],0);	CHECK_1(err, "dup3");
				// esegui comando 'cut -b 1-10'
				execlp("cut", "cut", "-b", "1-10", (char*)NULL);
				perror("execvp");	
				exit(errno);
			}else{
				// processo padre
				err = close(pipefd1[0]);	CHECK_1(err, "close6");
				err = close(pipefd2[0]);	CHECK_1(err, "close7");
				
				waitpid(pid1, NULL, 0);
				waitpid(pid2, NULL, 0);
				waitpid(pid3, NULL, 0);
			}
		}
	}
	
	return 0;
}
