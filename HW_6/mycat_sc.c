#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <util.h>

int main(int argc, char * argv[]) {

    if (argc == 1) {
	fprintf(stderr, "usa: %s file1 [fileN]\n", argv[0]);
	return EXIT_FAILURE;
    }
    
    char buffer[BUFSIZE];
    
    for(int i=1;i<argc;++i) {
	int fd;
	SYSCALL_NOEXIT(open, fd, open(argv[i],O_RDONLY), "Aprendo il file %s: errno=%d\n", argv[i], errno);
	if (fd < 0) continue;  // vado avanti anche se ci sono errori

	size_t len;
	do {
	    SYSCALL_NOEXIT(read,len,read(fd,buffer,BUFSIZE),"Leggendo il file %s : errno=%d\n", argv[i], errno);
	    if (len < 0) break; // vado avanti anche se ci sono errori
	    int r;
	    SYSCALL_NOEXIT(write,r, write(STDOUT_FILENO,buffer,len), "Scrivendo sullo stdout: errno=%d\n",errno);
	    if (r < 0)  break; // vado avanti anche se ci sono errori
	} while (len>0);

	SYSCALL(close,len,close(fd),"Chiudendo il file %s: errno=%d\n",argv[1],errno);
    }
    return 0;
}
