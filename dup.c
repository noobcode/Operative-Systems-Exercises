#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define CHECK_MENO_1(p,str) if((p) == -1){ perror(str); exit(EXIT_FAILURE); }

int main(){
	int fd;
	fd = open("pippo",O_WRONLY|O_TRUNC|O_CREAT, 0666);
	CHECK_MENO_1(fd, "open");
	CHECK_MENO_1(dup2(fd,1), "dup2");
	CHECK_MENO_1(close(fd), "close");
	printf("questo viene scritto su pippo\n");
	return 0;
}
