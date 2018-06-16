#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


void gestore(int signum){
}

int main(int argc, char* argv[]){
	int n, sec; // numero domande e timeout in secondi
	int i, k;
	int x, y;
	char result[10]; // numeri usati per le operazioni
	int giuste = 0, timeout = 0; // contatore risposte giuste & contatore timeout
	struct sigaction act;
	
	if(argc == 3){
		n = atoi(argv[1]);
		sec = atoi(argv[2]);
	}else {
		fprintf(stderr, "pochi argomenti\n");
		fflush(stdout);
		return 1;
	}
	
	memset(&act, 0, sizeof(act));
	act.sa_handler = gestore;
	if(sigaction(SIGALRM, &act, NULL) == -1){
		perror("sigaction");
		exit(-1);
	}
	
	for(i = 0; i < n; i++){
		x = rand() % 9;
		y = rand() % 9;
		fprintf(stdout,"%d*%d = ?\n", x, y);
		// parte il timer...	
		alarm(sec);
		// attende la risposta
		k = read(0, (void*)result, sizeof(int));
		alarm(0);
		
		if(k == -1 && errno == EINTR){ // se siamo stati interrotti da un segnale
			fprintf(stdout, "tempo scaduto\n");
			fflush(stdout);
			timeout++;
			continue;
		}
		if(k == -1 && errno != EINTR){
			fprintf(stdout, "errore\n");
			fflush(stdout);
			exit(EXIT_FAILURE);
		}
		result[k] = '\0';
		
		// se il risultato arriva entro il timeout stampa a video se il risultato è corretto o no
		if(atoi(result) == (x*y)){
			fprintf(stdout,"ok\n");
			fflush(stdout);
			giuste++;
		} else {
			fprintf(stdout,"no\n");
			fflush(stdout);
		}
	}	
	
	printf("il timeout è scaduto %d volte\nil numero di risposte esatte è %d\n", timeout, giuste);	
	return 0;
}
