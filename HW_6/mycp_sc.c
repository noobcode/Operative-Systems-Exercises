#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <util.h>

static inline ssize_t writeall(int fd, const void *buf, size_t count) {
    int len=0;
    if ((len=write(fd,buf,count)) != count) {
	if (len>=0) errno = EINTR; 
	return -1;
    }
    return count;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
	fprintf(stderr, "use: %s infile outfile [bufsize]\n", argv[0]);
	return EXIT_FAILURE;
    }
    char *infile  = argv[1];
    char *outfile = argv[2];
    size_t bufsize = BUFSIZE;
    if (argc >=4) bufsize = atol(argv[3]);
    
    int fin;
    SYSCALL(open, fin, open(infile, O_RDONLY),"Aprendo il file in input %s : errno=%d\n", infile, errno);	

    // imposto la maschera che voglio per la creazione dei file
    mode_t oldmask = umask(033);
    int fout;
    // creo il file con diritti 0640
    SYSCALL(open,fout,open(outfile, O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP),
	    "Creando il file in output %s : errno=%d\n", infile, errno);	
    // ripristino la vecchia maschera
    umask(oldmask);
    char *buf = malloc(bufsize*sizeof(char));
    if (!buf) { perror("malloc"); return errno;} 
    size_t len;
    do {
	SYSCALL(read,len,read(fin,buf,bufsize),"Leggendo il file in input %s:errno=%d\n",infile,errno);
	// se la write non scrive tutti i bytes lo considero come errore
	SYSCALL(writeall,len,writeall(fout,buf,len),"Scrivendo il file in output %s : errno=%d\n", outfile, errno);
    } while(len>0);
    SYSCALL(fsync,len,fsync(fout),"Facendo il flush del file in output %s : errno=%d\n", outfile, errno);
    SYSCALL(close,len,close(fin), "Chiudendo il file in input %s : errno=%d\n", infile, errno);
    SYSCALL(close,len,close(fout), "Chiudendo il file in output %s : errno=%d\n", outfile, errno);
    free(buf);
    return 0;
}
