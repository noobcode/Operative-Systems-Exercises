#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/times.h>
#include <errno.h>

#define CHECK_1(p,str) if((p) == -1){ int e = errno; perror(str); exit(e); }

int main(){
	char command[10];
	char arg[10];
	int pid;
	int status = 0;
	int count = 1;
	printf("welcome in DummyShell\n");
	
	while(1){
		printf("%d@dummy: stato = %d$ >>",count, status);
		scanf("%s", command);
		if(strcmp(command, "exit") == 0){
			printf("leaving DummyShell\n");
			exit(0);
		}
		scanf("%s", arg);
		CHECK_1(pid = fork(), "fork");
		if(pid == 0){
			// processo figlio, esegue 'command'
			execlp(command, command, arg, (char*)NULL);	
			perror("execlp");
			exit(1);
		}else{
			// processo padre
			waitpid(pid, &status, 0);
		}
		count++;
	}
	return 0;
}
