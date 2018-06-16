#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void lancia(int n){
	int pid, status, i;
	if(n > 0){
		for(i = 0; i < n; i++)	printf("-");
		printf("%d creo un processo figlio.\n", (int)getpid());
		fflush(stdout);
		
		pid = fork();
		if(pid == -1){
			perror("cannot fork");
			exit(EXIT_FAILURE);
		}
		if(pid){ /* padre */
			if(waitpid(pid, &status, 0) == -1){
				perror("wait");
				exit(EXIT_FAILURE);
			}
			if(WIFEXITED(status)){
				for(i = 0; i < n-1; i++)	printf("-");
				printf("%d: terminato con successo con stato %d\n",pid,WEXITSTATUS(status));
				return;
			}
		}
		if(pid == 0) /* figlio */
			lancia(n-1);

	}else
		printf("%d: sono l'ultimo.\n", (int)getpid());
		
}

int main(int argc, char* argv[]){
	int pid = getpid();
	int i,N;

	N = atoi(argv[1]);
	
	lancia(N);
	if(pid == getpid()){
		for(i = 0; i < N; i++)	printf("-");
		printf("%d: terminato con successo.\n", pid);
		fflush(stdout);
	}
	return 0;
}
