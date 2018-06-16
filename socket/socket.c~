/* un client e un server che comunicano via socket AF_UNIX. ogni famiglia di indirizzi ha le sue include e le sue strutture. è necessario cancellare il file SOCKNAME prima di rieseguire il codice. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>	// ind AF_UNIX

#define UNIX_PATH_MAX 108
#define SOCKNAME "./mysock"

#define CHECK_1(p, str) if((p == -1)) { int err = errno; perror(str); exit(err); }

int main(){
	int fd_skt, fd_c;
	int pid, k;
	char buff[10];
	struct sockaddr_un sa;
	strncpy(sa.sun_path, SOCKNAME, UNIX_PATH_MAX);
	sa.sun_family = AF_UNIX;
	
	pid = fork(); CHECK_1(pid, "fork");
	if(pid != 0){
		// padre, server.
		// creazione server socket
		fd_skt = socket(AF_UNIX, SOCK_STREAM, 0); 				CHECK_1(fd_skt, "socket");
		k = bind(fd_skt, (struct sockaddr*)&sa, sizeof(sa));	CHECK_1(k, "bind");
		k = listen(fd_skt, SOMAXCONN); 							CHECK_1(k, "listen");
		fd_c = accept(fd_skt, NULL, 0); 						CHECK_1(fd_c, "accept");
		k = read(fd_c, buff, sizeof(buff));						CHECK_1(k, "read");
		printf("server got: %s\n", buff);
		k = write(fd_c, "bye!", 5);								CHECK_1(k, "write");
		k = close(fd_c);										CHECK_1(k, "close");
		k = close(fd_skt);										CHECK_1(k, "close");
		exit(EXIT_SUCCESS);
	}else{
		// figlio, client
		// creazione client socket
		fd_skt = socket(AF_UNIX, SOCK_STREAM, 0);				CHECK_1(fd_skt, "socket");
		while( connect(fd_skt,(struct sockaddr*)&sa, sizeof(sa)) == -1){
			if(errno == ENOENT)	sleep(1); // sock non esiste
			else	exit(EXIT_FAILURE);
		}
		k = write(fd_skt, "ciao!",6);							CHECK_1(k, "write");
		k = read(fd_skt, buff, sizeof(buff));					CHECK_1(k, "read");
		printf("got %s\n",buff);
		k = close(fd_skt);										CHECK_1(k, "close");
		exit(EXIT_SUCCESS);
	}
	
	return 0;
}
