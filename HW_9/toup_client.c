#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <conn.h>
#include <assert.h>

int main(int argc, char *argv[]) {
    if (argc == 1) {
	fprintf(stderr, "usa: %s stringa [stringa]\n", argv[0]);
	exit(EXIT_FAILURE);
    }

    struct sockaddr_un serv_addr;
    int sockfd;
    SYSCALL(sockfd, socket(AF_UNIX, SOCK_STREAM, 0), "socket");
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sun_family = AF_UNIX;    
    strncpy(serv_addr.sun_path,SOCKNAME, strlen(SOCKNAME)+1);

    int notused;
    SYSCALL(notused, connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)), "connect");
    
    char buffer[BUFSIZE]; 
    for(int i=1; i<argc;++i) {
	assert(strlen(argv[i])<BUFSIZE);
	int n=strlen(argv[i])+1;

	/*  NOTA: provare ad utilizzare writev (man 2 writev) per fare un'unica SC */
	SYSCALL(notused, write(sockfd, &n, sizeof(int)), "write");
	SYSCALL(notused, write(sockfd, argv[i], n), "write");

	/* NOTA: provare ad utilizzare readv (man 2 readv) per fare un'unica SC */
	SYSCALL(notused, read(sockfd, &n, sizeof(int)), "read");
	SYSCALL(notused, read(sockfd, buffer, n), "read");
	buffer[n] = '\0';
	printf("result: %s\n", buffer);
    }
    close(sockfd);
    return 0;
}
