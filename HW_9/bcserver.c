#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/uio.h>

#include <conn.h>

void cleanup() {
    unlink(SOCKNAME);
}

int cmd(const char str[], char *buf) {
    int tobc[2];
    int frombc[2];

    int notused;
    SYSCALL(notused, pipe(tobc), "pipe1");
    SYSCALL(notused, pipe(frombc), "pipe2");

    if (fork() == 0) {
	SYSCALL(notused, close(tobc[1]), "close");
	SYSCALL(notused, close(frombc[0]), "close");
	SYSCALL(notused, dup2(tobc[0],0), "dup2 child (1)");
	SYSCALL(notused, dup2(frombc[1],1), "dup2 child (2)");

	execl("/usr/bin/bc", "bc", "-l", NULL);
	return -1;
    }

    SYSCALL(notused, close(tobc[0]), "close");
    SYSCALL(notused, close(frombc[1]), "close");

    int n;
    SYSCALL(n, write(tobc[1], str,strlen(str)), "write");
    SYSCALL(n, read(frombc[0], buf, BUFSIZE), "read");
    SYSCALL(notused, close(tobc[1]), "close");
    SYSCALL(notused, wait(NULL), "wait");
    return n;
}

int main(int argc, char *argv[]) {
    // cancello il socket file se esiste
    cleanup();
    // se qualcosa va storto ....
    atexit(cleanup);

    int listenfd;
    // creo il socket
    SYSCALL(listenfd, socket(AF_UNIX, SOCK_STREAM, 0), "socket");

    // setto l'indirizzo 
    struct sockaddr_un serv_addr;
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;    
    strncpy(serv_addr.sun_path, SOCKNAME, strlen(SOCKNAME)+1);

    int notused;
    // assegno l'indirizzo al socket 
    SYSCALL(notused, bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr)), "bind");
  
    // marco il socket come passivo e definisco un n. massimo di connessioni pendenti
    SYSCALL(notused, listen(listenfd, 1), "listen");

    int connfd, n;
    SYSCALL(connfd, accept(listenfd, (struct sockaddr*)NULL ,NULL), "accept");
    while(1) {      
	msg_t str;
	char buffer[BUFSIZE];
	      
	memset(str.str, '\0', BUFSIZE);
	// leggo la size della stringa
	SYSCALL(n, read(connfd, &str.len, sizeof(int)), "read1");
	// leggo la stringa
	SYSCALL(n, read(connfd, str.str, str.len), "read2");
	if (n==0) break;
	memset(buffer, '\0', BUFSIZE);
	if ((n = cmd(str.str, buffer)) < 0) { 
	    fprintf(stderr, "Errore nell'esecuzione del comando\n");
	    break;
	}
	buffer[n] = '\0';

	// invio la risposta
	struct iovec iov[2] = { {&n, sizeof(int)}, {buffer, n} };
	SYSCALL(notused, writev(connfd, iov, 2), "writev");
    }
    close(connfd);   
    printf("connection done\n");
    return 0;
}
