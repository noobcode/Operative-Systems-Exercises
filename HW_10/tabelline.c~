#define _GNU_SOURCE 1
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define MAX 10

// il signal handler non fa nulla, serve per 
// interrompere la chiamata di sistema read 
static void sighandler(int sig) {
    ;
}

int main(int argc, char *argv[]) {
    if(argc<3) {
	fprintf(stderr, "%s numprove timeout(sec)\n", argv[0]);
	return(EXIT_FAILURE);
    }
    int numprove = atoi(argv[1]);
    int timeout  = atoi(argv[2]);

    // installo il signal handler per SIGALRM
    struct sigaction sa;
    memset (&sa, 0, sizeof(sa));
    sa.sa_handler = sighandler;
    if (sigaction(SIGALRM,  &sa, NULL) == -1) 
	perror("sigaction SIGALRM");
    
    int scaduti=0, ok=0;
    char line[32];
    for(int i=0;i<numprove;++i) {
	int n1 = rand() % MAX;          
	int n2 = rand() % MAX;
	snprintf(line,32,"%d * %d ?",n1,n2);
	fprintf(stdout, "%s\n", line);
	fflush(stdout);

	alarm(timeout);
	int r = read(0,line,32);
	alarm(0);                   // spegniamo il timer -- sperando di fare in tempo!
	if(r == -1 && errno == EINTR) { // la read e' stata interrotta
	    fprintf(stdout, "Timeout scaduto. La risposta e': %d\n", n1*n2);
	    fflush(stdout);
	    ++scaduti;
	    continue;
	}
	if(r == -1 && errno != EINTR) { // errore 
	    perror("read");
	    return(EXIT_FAILURE);
	}
	line[r] = '\0';
	if(atoi(line) == n1*n2) {  
	    fprintf(stdout, "Risposta esatta\n");
	    fflush(stdout);
	    ++ok;
	} else {
	    fprintf(stdout, "Risposta sbagliata, il risultato corretto e': %d\n", n1*n2);
	    fflush(stdout);
	}
    } 

    fprintf(stdout,"Hai indovinato il %2.0f%% delle domande\n", 100.0*(ok/(float)numprove));
    if(scaduti != 0)
	fprintf(stdout, "Ma il timeout è scaduto per %d domande\n",scaduti);
    return 0;
}



