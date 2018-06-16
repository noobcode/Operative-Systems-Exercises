#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define N 32

int main(){
	int pfd[2], pid, l;
	char msg[N] = "sono proprio un lerciume totale";
	if(pipe(pfd) == -1){
		perror("pipe");
		exit(1);
	}
	if((pid = fork()) == -1){
		perror("fork");
		exit(1);
	}
	if(pid){ // padre
		close(pfd[1]); // chiude scrittura
		l = read(pfd[0],msg,N);
		if(l == -1){
			perror("read");
			exit(1);
		}
		printf("%d padre: ho ricevuto: %s\n",(int)getpid(),msg);
		close(pfd[0]); // chiude lettura
	}else{ // figlio
		close(pfd[0]); // chiude lettura
		l = write(pfd[1],msg,N);
		if(l == -1){
			perror("write");
			exit(1);
		}
		close(pfd[1]); //chiude scrittura
		printf("%d figlio, messaggio inviato\n", getpid());
	}
}
