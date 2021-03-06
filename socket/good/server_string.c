#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/un.h> // ind AF_UNIX
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <ctype.h>
#define LEN 100
#define MAXCONN 32
#define SOCKNAME "./mysocket"
#define SYS_CALL(p, str) if((p) == -1) { perror(str); exit(EXIT_FAILURE); }

void cleanup() {
    unlink(SOCKNAME);
}

void* worker(void* arg){
	assert(arg);
	char buff[LEN];
	int i, n, len;
	int client_fd = (int)arg;
	while(1){
		memset(buff, '\0', LEN);
		SYS_CALL(n = read(client_fd, &len, sizeof(int)), "read worker1");
		if(n == 0)	break;
		SYS_CALL(n = read(client_fd, buff, len), "read worker2");	
		for(i = 0; i < n; i++)
			if(islower(buff[i]))	
				buff[i] = toupper(buff[i]);
		buff[n] = '\0';
		SYS_CALL(n = write(client_fd, &len, sizeof(int)),"write worker1");
		SYS_CALL(n = write(client_fd, buff, len),"write worker2");
	}
	close(client_fd);
	pthread_exit((void*)0);
}

void spawn_thread(int client_fd){
	pthread_attr_t thattr;
    pthread_t thid;
	// inizializzo thattr con attributi di default, possiamo passare questo oggetto 
	// nella pthread_create
    if (pthread_attr_init(&thattr) != 0) {
		fprintf(stderr, "pthread_attr_init FALLITA\n");
		close(client_fd);
		return;
    }
    // setto il thread in modalità detached.
    // setta il detach state attribute del thread attribute obect 'thattr' nel valore 
    // PTHREAD_CREATE_DETACHED. 
	// determina se il thread che verrà creato sarà joinable o in modalità detached.
    if (pthread_attr_setdetachstate(&thattr,PTHREAD_CREATE_DETACHED) != 0) {
		fprintf(stderr, "pthread_attr_setdetachstate FALLITA\n");
		pthread_attr_destroy(&thattr);
		close(client_fd);
		return;
    }
    if (pthread_create(&thid, &thattr, worker, (void*)client_fd) != 0) {
		fprintf(stderr, "pthread_create FALLITA");
		pthread_attr_destroy(&thattr);
		close(client_fd);
		return;
    }
}

int main(){
	struct sockaddr_un sa;
	int sock_fd, client_fd;
	int err;
	/*-----------*/
	cleanup();
	err = atexit(cleanup);
	if (err != 0) {
    	fprintf(stderr, "cannot set exit function\n");
        exit(EXIT_FAILURE);
    }
    /*-----------*/
    SYS_CALL(sock_fd = socket(AF_UNIX, SOCK_STREAM, 0), "socket");
	memset(&sa, '0', sizeof(sa));
	sa.sun_family = AF_UNIX;
	strncpy(sa.sun_path, SOCKNAME, strlen(SOCKNAME)+1);
	SYS_CALL(bind(sock_fd, (struct sockaddr*)&sa, sizeof(sa)), "bind");
	SYS_CALL(listen(sock_fd, MAXCONN), "listen");
	
	while(1){
		SYS_CALL(client_fd = accept(sock_fd, (struct sockaddr*)NULL, 0), "accept");
		spawn_thread(client_fd);			
	}
	close(sock_fd);
	return 0;
}
