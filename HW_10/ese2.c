#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#define SYS_CALL(k, str) if((k) == -1) { perror(str); }

// l'accesso alle variabili globali non è safe nell'handler, quindi vanno dichiarate cosi
static volatile sig_atomic_t count_sigint = 0;
static volatile sig_atomic_t count_sigtstp = 0;
static volatile sig_atomic_t flag = 0;

void handler(int signum){
	if(signum == SIGINT) { 
		count_sigint++;
	} else if(signum == SIGTSTP){
		flag = 1;
		count_sigtstp++;
	} else if(signum == SIGALRM){
		_exit(EXIT_SUCCESS);
	}
}

int main(){
	struct sigaction act;
	int k;
	char buff;
	sigset_t mask, oldmask;
	
	// azzero la maschera e poi metto a 1 i bit corrispondenti a SIGINT e SIGTSTP
	sigemptyset(&mask);
	sigaddset(&mask, SIGINT);
	sigaddset(&mask, SIGTSTP);
	
	// nello startup del programma è bene mascherare i segnali finché non abbiamo registrato tutte le sigaction
	SYS_CALL(sigprocmask(SIG_BLOCK, &mask, &oldmask), "sigprocmask");
	
	memset(&act, 0, sizeof(act));
	act.sa_handler = handler;
	SYS_CALL(sigaction(SIGINT, &act, 0), "sigaction1");
	SYS_CALL(sigaction(SIGTSTP, &act, 0), "sigaction2");
	SYS_CALL(sigaction(SIGALRM, &act, 0), "sigaction3");
	
	// ripristino vecchia maschera
	SYS_CALL(sigprocmask(SIG_SETMASK, &oldmask, 0), "sigprocmask");
	
	while(1){
		pause();
		if(flag){
			fprintf(stdout, "numero SIGINT %d\n", count_sigint);
			fflush(stdout);
			flag = 0;
			count_sigint = 0;
		}
			
		if(count_sigtstp == 3){
			count_sigtstp = 0;
			fprintf(stdout, "terzo SIGTSTP, premi un tasto per continuare...\n");
			fflush(stdout);
			alarm(5);
			
			k = read(0, &buff, sizeof(buff));
			if(k == -1 && errno == EINTR){
				perror("read");
			}
			alarm(0);
		}
	}
	return 0;
}
