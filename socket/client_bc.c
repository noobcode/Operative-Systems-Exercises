#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#define N 20
#define SOCKNAME "socket_bc"

int main(){
	int server_sockfd;
	int len, result;
	struct sockaddr_un address;
	char buff[N];
	
	server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(server_sockfd == -1){ perror("socket"); exit(errno); }
	
	address.sun_family = AF_UNIX;
	strcpy(address.sun_path, SOCKNAME);
	len = strlen(SOCKNAME) + sizeof(int);
	
	result = connect(server_sockfd, (struct sockaddr*)&address, len);
	if(result == -1){ perror("connect"); exit(errno); }
	
	while(1){
		if(fgets(buff, N, stdin) == NULL){ fprintf(stderr, "fgets failed\n"); exit(1); }
		if(strcmp(buff, "quit") == 0) break;			
				
		if(write(server_sockfd, buff, N) == -1) { perror("write"); exit(errno); }
		printf("inviato %s\n", buff);
		if(read(server_sockfd, buff, N) == -1) { perror("read"); exit(errno); }
		printf("%.d\n", atoi(buff));	
	}
	
	if(close(server_sockfd) == -1) { perror("close"); exit(errno); }
	unlink(SOCKNAME);
	return 0;		
}
