#if !defined(CONN_H)
#define CONN_H

#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#if !defined(BUFSIZE)
#define BUFSIZE 1024
#endif

#define SOCKNAME     "./cs_sock"
#define MAXBACKLOG   32

#define SYSCALL(r,c,e) \
    if((r=c)==-1) { perror(e);exit(errno); }

typedef struct msg {
    int len;
    char str[BUFSIZE];
} msg_t;

#endif /* CONN_H */
