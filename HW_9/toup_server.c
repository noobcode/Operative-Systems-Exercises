#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>

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

void *threadF(void *arg) {
    assert(arg);
    long connfd = (long)arg;
    do {
	msg_t str;
	memset(str.str, '\0', BUFSIZE);
	int n;
	SYSCALL(n, read(connfd, &str.len, sizeof(int)), "read1");
	if (n==0) break;
	SYSCALL(n, read(connfd, str.str, str.len), "read2");

	toup(str.str);

	SYSCALL(n, write(connfd, &str, sizeof(int)+str.len), "write");
    } while(1);
    close(connfd);	    
    return NULL;
}

void spawn_thread(long connfd) {
    pthread_attr_t thattr;
    pthread_t thid;
    
    if (pthread_attr_init(&thattr) != 0) {
	fprintf(stderr, "pthread_attr_init FALLITA\n");
	close(connfd);
	return;
    }
    // settiamo il thread in modalità detached
    if (pthread_attr_setdetachstate(&thattr,PTHREAD_CREATE_DETACHED) != 0) {
	fprintf(stderr, "pthread_attr_setdetachstate FALLITA\n");
	pthread_attr_destroy(&thattr);
	close(connfd);
	return;
    }
    if (pthread_create(&thid, &thattr, threadF, (void*)connfd) != 0) {
	fprintf(stderr, "pthread_create FALLITA");
	pthread_attr_destroy(&thattr);
	close(connfd);
	return;
    }
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

    while(1) {      
	long connfd;
	SYSCALL(connfd, accept(listenfd, (struct sockaddr*)NULL ,NULL), "accept");
	//printf("connection accepted\n");    	
	spawn_thread(connfd);
    }
    return 0;
}
