/* vogliamo ottenere la pagina HTTP che corrisponde a www.yahoo.com chiedendola direttamente
al web server di 216.119.125.70 */
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define REQUEST "GET / HTTP/1.0\r\n\r\n"
#define N 1024
int main(){
	int fd_skt, nread;
	struct sockaddr_in sa;
	char buff[N];
	sa.sin_family = AF_INET;
	// numero di porta (uint16_t) deve essere in network byte order
	sa.sin_port = htons(80);
	// indirizzo IPv4 (uint32_t) deve essere in network byte order 216.119.125.70
	//sa.sin_addr.s_addr = htonl(216<<24) + htonl(119<<16) + htonl(125<<8) + htonl(70);
	// oppure, meglio cosi
	sa.sin_addr.s_addr = inet_addr("216.109.125.70");
	fd_skt = socket(AF_INET, SOCK_STREAM, 0);
	connect(fd_skt, (struct sockaddr*)&sa, sizeof(sa));
	write(fd_skt, REQUEST, strlen(REQUEST));
	nread = read(fd_skt, buff, sizeof(buff));
	write(1, buff, nread);
	close(fd_skt);
	
	return 0;
}
