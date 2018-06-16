#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define CHECK_MENO_1(p,str) if((p) == -1){ perror(str); exit(EXIT_FAILURE); }

void who_wc(){
	int pfd[2];
	int pid1, pid2;
	CHECK_MENO_1(pipe(pfd), "pipe");
	
	// creazione figlio 'who'
	CHECK_MENO_1(pid1 = fork(), "fork");
	if(pid1 == 0){ /* figlio1: esegue who */
		CHECK_MENO_1(dup2(pfd[1],1), "dup2");
		CHECK_MENO_1(close(pfd[0]),"close");
		CHECK_MENO_1(close(pfd[1]),"close");
		execlp("who", "who", (char*) NULL);
	}
	
	// creazione figlio 'wc'
	CHECK_MENO_1(pid2 = fork(), "fork");
	if(pid2 == 0){ /* figlio2: esegue wc */
		CHECK_MENO_1(dup2(pfd[0],0), "dup2");
		CHECK_MENO_1(close(pfd[1]),"close");
		CHECK_MENO_1(close(pfd[0]),"close");
		execlp("wc", "wc", "-l", (char*) NULL);
	}  
	// padre
	CHECK_MENO_1(close(pfd[1]),"close");
	CHECK_MENO_1(close(pfd[0]),"close");
	waitpid(pid1,NULL,0);
	waitpid(pid2,NULL,0);
}

int main(){
	who_wc();
	return 0;
}
