#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#define CHECK_MENO_1(p,str) if((p) == -1) { perror(str); exit(EXIT_FAILURE); }

void zombie(int n){
	int pid;
	if(n > 0){
		CHECK_MENO_1(pid = fork(), "fork");
		if(pid){ /* padre */
			zombie(n-1);
		}else{ /* figlio */
			//printf("%d padre --> %d zombie\n", (int)getppid(), (int)getpid());
			exit(1);
		}
	}
}

int main(int argc, char* argv[]){
	int arg, pid, i; 
	
	if(argc != 2){
		fprintf(stderr, "usage: %s <num>\n", argv[0]);
		return EXIT_FAILURE;
	}
	
	arg = atoi(argv[1]);
	zombie(arg);
	sleep(10);
	return 0;
}
