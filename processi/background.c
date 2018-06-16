#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
	printf("ciao\n");
	daemon(0,1);	
	sleep(5);
	printf("figlio: %d padre: %d\n",getpid(), getppid());
			
	return 0;
}
