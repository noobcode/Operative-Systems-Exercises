#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <conn.h>


int main(int argc, char *argv[]) {
    struct sockaddr_un serv_addr;
    int sockfd;
    SYSCALL(sockfd, socket(AF_UNIX, SOCK_STREAM, 0), "socket");
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sun_family = AF_UNIX;    
    strncpy(serv_addr.sun_path,SOCKNAME, strlen(SOCKNAME)+1);

    int notused;
    SYSCALL(notused, connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)), "connect");
    
    do {
	msg_t str;
	char buffer[BUFSIZE]; 
   
	memset(str.str, '\0', BUFSIZE);
	if (fgets(str.str, BUFSIZE-1, stdin) == NULL) break;
	if (strncmp(str.str, "quit", 4)== 0) break;
	str.len = strlen(str.str);
	
	// scrivo con un'unica write l'intero messaggio 
	SYSCALL(notused, write(sockfd, &str, sizeof(int)+str.len), "write");
	int n, notused;
	/*
	 *  NOTA: provare ad utilizzare readv (man 2 readv) per fare un'unica SC
	 */
	SYSCALL(notused, read(sockfd, &n, sizeof(int)), "read");
	SYSCALL(notused, read(sockfd, buffer, n), "read");
	buffer[n] = '\0';

	printf("result: %s\n", buffer);
	
    } while(1);
    
    close(sockfd);
    return 0;
}
