#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <sys/un.h> // ind AF_UNIX
#define LEN 100
#define SOCKNAME "./mysocket"
#define SYS_CALL(p, str) if((p) == -1) { perror(str); exit(EXIT_FAILURE); }

int main(int argc, char* argv[]){
	struct sockaddr_un sa;
	int sock_fd;
	int i, n;
	char buff[LEN];
	
	if(argc == 1){
		fprintf(stderr, "no argomenti\n");
		exit(EXIT_FAILURE); 
	}
	SYS_CALL(sock_fd = socket(AF_UNIX, SOCK_STREAM, 0), "socket)");
	memset(&sa, '0', sizeof(sa));
	sa.sun_family = AF_UNIX;
	strncpy(sa.sun_path, SOCKNAME, strlen(SOCKNAME)+1);
	
	/*while(connect(sock_fd, (struct sockaddr*)&sa, sizeof(sa)) == -1)
		if(errno == ENOENT) // sock non esiste
			sleep(1);
		else
			exit(EXIT_FAILURE);*/
	SYS_CALL(connect(sock_fd, (struct sockaddr*)&sa, sizeof(sa)),"connect");
			
	for(i = 1; i < argc; i++){		
		assert(strlen(argv[i]) < LEN);
		n = strlen(argv[i]) + 1;
		SYS_CALL(write(sock_fd, &n, sizeof(int)), "write1");
		SYS_CALL(write(sock_fd, argv[i], n), "write2");
		
		SYS_CALL(read(sock_fd, &n, sizeof(int)), "read 1 client");
		SYS_CALL(read(sock_fd, buff, n), "read 2 client");
		buff[n] = '\0';
		printf("processo %d ricevuto %s\n", getpid(), buff);
	}
	
	close(sock_fd);
	return 0;
}
