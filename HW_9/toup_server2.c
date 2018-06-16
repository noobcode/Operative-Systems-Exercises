#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/select.h>

#include <conn.h>

void cleanup() {
    unlink(SOCKNAME);
}

// converte tutti i carattere minuscoli in maiuscoli
void toup(char *str) {
    char *p = str;
    while(*p != '\0') { 
        *p = (islower(*p)?toupper(*p):*p); 
	++p;
    }        
}

int cmd(long connfd) {
    msg_t str;
    memset(str.str, '\0', BUFSIZE);
    if (read(connfd, &str.len, sizeof(int))<=0) return -1;
    if (read(connfd, str.str, str.len)<=0) return -1;
    toup(str.str);
    if (write(connfd, &str, sizeof(int)+str.len)<0) return -1;
    return 0;
}

// aggiorna il massimo id del descrittore
int updatemax(fd_set set, int fdmax) {
    for(int i=(fdmax-1);i>=0;--i)
	if (FD_ISSET(i, &set)) return i;
    return -1;
}

int main(int argc, char *argv[]) {
    cleanup();    
    atexit(cleanup);  

    int listenfd;
    SYSCALL(listenfd, socket(AF_UNIX, SOCK_STREAM, 0), "socket");
    
    struct sockaddr_un serv_addr;
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;    
    strncpy(serv_addr.sun_path, SOCKNAME, strlen(SOCKNAME)+1);

    int notused;
    SYSCALL(notused, bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr)), "bind");
    SYSCALL(notused, listen(listenfd, MAXBACKLOG), "listen");
    
    fd_set set, tmpset;
    // azzero sia il master set che il set temporaneo usato per la select
    FD_ZERO(&set);
    FD_ZERO(&tmpset);

    // aggiungo il listener fd al master set
    FD_SET(listenfd, &set);

    // tengo traccia del file descriptor con id piu' grande
    int fdmax = listenfd; 

    while(1) {      
	// copio il set nella variabile temporanea per la select
	tmpset = set;
	int notused;
	SYSCALL(notused, select(fdmax+1, &tmpset, NULL, NULL, NULL), "select");

	// cerchiamo di capire da quale fd abbiamo ricevuto una richiesta
	for(int i=0; i <= fdmax; i++) {
	    if (FD_ISSET(i, &tmpset)) {
		long connfd;
		if (i == listenfd) { // e' una nuova richiesta di connessione 
		    SYSCALL(connfd, accept(listenfd, (struct sockaddr*)NULL ,NULL), "accept");
		    FD_SET(connfd, &set); 
		    if(connfd > fdmax) fdmax = connfd; 
		} else connfd = i;  // e' una nuova richiesta da un client già connesso

		// eseguo il comando e se c'e' un errore lo tolgo dal master set
		if (cmd(connfd) < 0) { 
		    close(connfd); 
		    FD_CLR(connfd, &set); 
		    // devo reinpostare il massimo ?
		    if (connfd == fdmax) fdmax = updatemax(set, fdmax);
		}
	    }
	}
    }
    // not reached
    close(listenfd);
    return 0;
}
