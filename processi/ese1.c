#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#define N 100


int main(){
	int pid1, pid2, err;
	int pipefd[2];
	int msg, i;
	
	srand(time(NULL));
	
	// inizializzo pipe
	if(pipe(pipefd) == -1){
		err = errno;
		perror("pipe");
		exit(err);
	}
	
	// fork
	if( (pid1 = fork()) == -1 ){
		fprintf(stderr, "cannot fork\n");
		exit(1);
	}
	
	if(pid1){
		//processo padre
		if( (pid2 = fork()) == -1 ){
			fprintf(stderr, "cannot fork\n");
			exit(1);
		}
		if(pid2 == 0){
			// consumatore
			if(dup2(pipefd[0],0) == -1){ err = errno; perror("dup1"); exit(err); }
			if(close(pipefd[0]) == -1){ err = errno; perror("close3"); exit(err); }
			if(close(pipefd[1]) == -1){ err = errno; perror("close4"); exit(err); }
			for(i = 0; i < N; i++){
			if(read(0, &msg, sizeof(msg)) == -1 ){ 
				err = errno;
				perror("read");
				exit(err);
			}
			fprintf(stdout, "ricevuto %d\n", msg);
			fflush(stdout);
			}
		}
	}else{
		// produttore
		if(dup2(pipefd[1],1) == -1){ err = errno; perror("dup2"); exit(err); }
		if(close(pipefd[0]) == -1){ err = errno; perror("close5"); exit(err); }
		if(close(pipefd[1]) == -1){ err = errno; perror("close6"); exit(err); }
		for(i = 0; i < N; i++){
			//msg = rand() % N;
			msg = i;
			if(write(1, &msg, sizeof(msg)) == -1 ){ 
				err = errno;
				perror("write");
				exit(err);
			}
		}
	}
	
	if(pid1 && pid2){
		if(close(pipefd[0]) == -1){ err = errno; perror("close1"); exit(err); }	
		if(close(pipefd[1]) == -1){ err = errno; perror("close2"); exit(err); }
	}
	printf("done\n");
	return 0;
}


