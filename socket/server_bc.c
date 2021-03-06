#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#define N 20
#define MAXCONN 5
#define SOCKNAME "socket_bc"

int cmd(char* buff, int client_sockfd){
	int tobc[2];
	int frombc[2];
	int pid, result;
	
	if(pipe(tobc) == -1){ perror("pipe"); exit(errno); }
	if(pipe(frombc) == -1){ perror("pipe"); exit(errno); }
	
	pid = fork();
	if(pid == 0){
		// figlio, esegue 'bc'
		close(tobc[1]);
		close(frombc[0]);
		dup2(tobc[0], 0);
		dup2(frombc[1], 1);
		execlp("bc", "bc", (char*)NULL);
		return -1;
	}else{
		// padre invia buff come input al figlio
		close(tobc[0]);
		close(frombc[1]);
		result =  write(tobc[1], buff, N);
		result = read(frombc[0], buff, N);
		waitpid(pid, NULL, 0);
		return result;
	}
}

int main(){
	int server_sockfd, client_sockfd;
	int len, client_len, result;
	char buff[N];
	struct sockaddr_un address;
	struct sockaddr_un client_address;
	
	unlink(SOCKNAME);
	server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(server_sockfd == -1){ perror("socket"); exit(errno); }
	
	address.sun_family = AF_UNIX;
	strcpy(address.sun_path, SOCKNAME);
	len = strlen(SOCKNAME) + sizeof(int);
	
	if(bind(server_sockfd, (struct sockaddr*)&address, len) == -1){ perror("bind"); exit(errno); }
	if(listen(server_sockfd, MAXCONN) == -1){ perror("listen"); exit(errno); }	
	printf("server initialized\n");
	
	while(1){
		client_len = sizeof(client_address);
		client_sockfd = accept(server_sockfd, (struct sockaddr*) &client_address, (socklen_t*)&client_len);
		
		result = cmd(buff, client_sockfd);
		if(result == -1){ fprintf(stderr, "errore esecuzione comando\n"); exit(-1); }
		write(client_sockfd, buff, N);	
	}
	return 0;
}
