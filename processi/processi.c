#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>

// utility macro
#define SYSCALL(r,c,e) \
    if((r=c)==-1) { perror(e);exit(errno); }

// numero di processi di default
const int NPROC = 2;

// forward declaration, procedura eseguita da un processo
void procF(int);

int main(int argc, char * argv[]) {
    int nproc;
    
    // check arguments
    if(argc>1)  nproc = atoi(argv[1]);
    else  nproc = NPROC;
    
    // per distinguere tra il processo padre ed i processi figli
    int ppid = getpid();
    
    // genero nproc processi figli
    for(int i = 0; i < nproc; i++) {
		int r;
	
		SYSCALL(r,fork(),"creando un processo");
		if(r == 0) {               // processo figlio
	    	procF(i+2);            // eseguo i+2 iterazioni
	    	break;                 // altrimenti genero troppi processi
		} else {                   // processo padre
	    	printf("Creato processo figlio con pid %d\n",r);
	                           // continua il ciclo
		}
    } 
    
    // il solo processo padre attende i figlio
    if(getpid() == ppid) {
        int status,r;
		for(int i = 0; i < nproc; i++) {
	    	SYSCALL(r, wait(&status), "wait");
	    	if(WIFEXITED(status))
				printf("Figlio con pid %d terminato con exit, codice %d\n", r, WEXITSTATUS(status));
	    	else if (WIFSIGNALED(status)) {
				printf("Figlio con pid %d terminato da un segnale (sig=%d)\n", r, WTERMSIG(status));
	    	} else if (WIFSTOPPED(status) || WIFCONTINUED(status)) 
				printf("Figlio con pid %d interrotto da o ripartito con un segnale\n",r);
			}
    } // else sono un processo figlio e termino senza fare nient'altro
    
    return getpid() % 256; // termino con questo codice
}

// procedura eseguita dai processi figli
void procF(int niter) {
  for(int i = 0; i < niter; ++i) {
    printf("Sono il processo %d all'iterazione %d\n",getpid(),i);
    sleep(1);
  }
}
