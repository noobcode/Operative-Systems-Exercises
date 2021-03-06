#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>


int main(){
	char ch = 'B';
	int socket_fd;
	int result, len;
	struct sockaddr_un socket_address;
	
	socket_address.sun_family = AF_UNIX;
	strcpy(socket_address.sun_path, "server_socket"); 
	len = sizeof(socket_address);
	socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	result = connect(socket_fd, (struct sockaddr*)&socket_address, len);
	if(result == -1){
		perror("connect failed");
		exit(EXIT_FAILURE);
	}
	
	write(socket_fd, &ch, sizeof(ch));
	read(socket_fd, &ch, sizeof(ch));
	printf("client2 got: %c\n", ch);

	close(socket_fd);
	return 0;
}
