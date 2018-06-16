#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <unistd.h>



int main(){
	int server_sockfd, client_sockfd;
	int server_len, client_len;
	struct sockaddr_un server_address;
	struct sockaddr_un client_address;
	
	// remove any old socket and create an unnamed socket for the server
	unlink("server_socket");
	server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	
	//name the socket
	server_address.sun_family = AF_UNIX;
	strcpy(server_address.sun_path, "server_socket");
	server_len = sizeof(server_address);
	bind(server_sockfd, (struct sockaddr*)&server_address, server_len);
	
	// create a connection queue and wait for clients
	listen(server_sockfd, 5);
	printf("server initialized\n");
	while(1){
		char ch;
		
		// accept a connection
		client_len = sizeof(client_address);
		client_sockfd = accept(server_sockfd, (struct sockaddr*)&client_address, (socklen_t*)&client_len);
	
	// we can now read/write to client on client_sockfd
	read(client_sockfd, &ch, 1);
	ch++;
	write(client_sockfd, &ch, 1);
	close(client_sockfd);
	
	}
	return 0;
}
