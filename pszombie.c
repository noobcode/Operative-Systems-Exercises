#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#define CHECK_MENO_1(p,str) if((p) == -1){ perror(str); exit(EXIT_FAILURE); }

/* stampa il pid dei processi zombie presenti nel sistema utiliazzando una pipeline di 3 processi. il primo processo esegue il comando 'ps -A -ostat,pid' e fornisce l'input la secondo processo che esegue il comando 'grep -e [zZ]' e che a sua volta fornisce l'input al terzo processo che esegue il comando 'awk {print $2}'.

ps -A -ostat,pid | grep -e [zZ] | awk '{print $2}' */

int main(){
	int pipe1[2];
	int pipe2[2];
	int pid1, pid2, pid3;
	int status;
	
	CHECK_MENO_1(pipe(pipe1), "pipe1");
	
	CHECK_MENO_1(pid1 = fork(), "fork");
	/* figlio 1 */
	if(!pid1){ 
		CHECK_MENO_1(dup2(pipe1[1],1),"dup2"); // redirigo stdout sull'ingresso di scrittura di pipe1
		CHECK_MENO_1(close(pipe1[0]), "close");	// chiude lettura
		CHECK_MENO_1(close(pipe1[1]), "close");
		
		execlp("ps", "ps", "-A", "-ostat,ppid", NULL);	
		perror("execlp1");								
		exit(errno);									
	}else{ // padre 
		CHECK_MENO_1(dup2(pipe1[0],0), "dup2"); // redigo lo stdin sull'ingresso di lettura di pipe1
		CHECK_MENO_1(close(pipe1[1]), "close"); // chiude scrittura
		CHECK_MENO_1(pipe(pipe2), "pipe2");	// crea pipe2
		CHECK_MENO_1(pid2 = fork(), "fork"); // crea un altro figlio
		if(!pid2){ // figlio 2
			CHECK_MENO_1(dup2(pipe2[1],1), "dup2"); //redirige stdout nell'ingresso scrittura pipe2
			CHECK_MENO_1(close(pipe2[0]), "close"); // chiude lettura
			execlp("grep", "grep", "-e", "[zZ]", NULL);
			perror("execvp2");
			exit(errno);
		}else{ // padre
			CHECK_MENO_1(dup2(pipe2[0],0), "dup2");
			CHECK_MENO_1(close(pipe2[1]), "close");
			CHECK_MENO_1(pid3 = fork(), "fork"); // fa un altro figlio
			if(!pid3){ /* figlio 3 */
				execlp("awk", "awk", "\"{print $2}\"", NULL);
				perror("execlp3");
				exit(errno);
			}
		}	
	}
	
	// chiudo descrittori padre inutilizzati
	
	
	// fai le waitpid
	CHECK_MENO_1(waitpid(pid1, &status, 0), "waitpid1");
	CHECK_MENO_1(waitpid(pid2, &status, 0), "waitpid2");
	CHECK_MENO_1(waitpid(pid3, &status, 0), "waitpid3");
	return 0;
}
