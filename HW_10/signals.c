#define _GNU_SOURCE 1
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

static volatile sig_atomic_t sigintcounter = -1;
static volatile sig_atomic_t sigstpcounter = -1;
static volatile sig_atomic_t sigstpflag    =  0;

static void sighandler(int sig) {
    
    switch(sig) {
    case SIGINT: {
	if (sigintcounter == -1) sigintcounter = 0;
	++sigintcounter;
    } break;
    case SIGTSTP: {
	sigstpflag = 1;
	if (sigstpcounter == -1) sigstpcounter = 0;
	++sigstpcounter;
    } break;
    case SIGALRM: {
	_exit(0);
    } break;
    default:{	
	abort();
    }
    }
}


int main() {

    sigset_t mask, oldmask;
    sigemptyset(&mask);   // resetto tutti i bits
    sigaddset(&mask, SIGINT);  // aggiunto SIGINT alla machera
    sigaddset(&mask, SIGTSTP); // aggiunto SIGTSTP alla machera

    // blocco i segnali SIGINT e SIGTSTP finche' non ho finito
    // l'installazione degli handler -- mi conservo la vecchia maschera
    if (sigprocmask(SIG_BLOCK, &mask, &oldmask) == -1) {
	perror("sigprocmask");
	return EXIT_FAILURE;
    }
   
    // installo il signal handler per tutti i segnali che mi interessano
    struct sigaction sa;
    // resetto la struttura
    memset (&sa, 0, sizeof(sa));   
    sa.sa_handler = sighandler;
    if (sigaction(SIGINT,  &sa, NULL) == -1) 
	perror("sigaction SIGINT");
    if (sigaction(SIGTSTP, &sa, NULL) == -1) 
	perror("sigaction SIGSTOP");
    if (sigaction(SIGALRM, &sa, NULL) == -1)
	perror("sigaction SIGALRM");
    
    while(1) {
	// atomicamente setta la maschera e si sospende
	if (sigsuspend(&oldmask) == -1 && errno != EINTR) {
	    perror("sigsuspend");
	    return (EXIT_FAILURE);
	}
	// qui i segnali sono nuovamente bloccati

	if (sigstpflag) {
	    printf("Ricevuti %d SIGINT\n", sigintcounter);
	    sigstpflag = 0;
	    sigintcounter = 0;
	} 
	if (sigstpcounter == 3) {
	    sigstpcounter = 0;
	    printf("Per continuare premi un tasto, altrimenti verrai terminato entro 10 secondi");
	    fflush(stdout);
	    alarm(10);
	    char c;
	    if (read(0, &c, sizeof(char)) == -1 && errno != EINTR) {
		perror("read");
		return EXIT_FAILURE;
	    }
	    alarm(0); // resetto l'allarme..... potrei non farcela
	}
    }

}
