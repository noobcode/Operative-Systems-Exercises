#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>        
#include <sys/types.h>       
#include <unistd.h>          
#include <errno.h>           

// utility macro
#define SYSCALL(r,c,e) \
    if((r=c)==-1) { perror(e);exit(errno); }

int main(int argc, char *argv[]) {
    if (argc == 1) {
		fprintf(stderr, "usa: %s comando [argomenti-comando]\n", argv[0]);
		return EXIT_FAILURE;
    }
    
    int pid;
    SYSCALL(pid, fork(), "fork");
  
    if(pid == 0) { // figlio 
		int r;
		execvp(argv[1],&argv[1]);
		perror("execvp");
		return errno;
    } 
    
    int status;
    SYSCALL(pid, wait(&status),"wait");
    printf("Processo %d terminato con ",pid);
    if(WIFEXITED(status))  
    	printf("exit(%d)\n",WEXITSTATUS(status));
    else 
    	printf("un segnale (sig=%d)\n", WTERMSIG(status));
    return 0;
}
