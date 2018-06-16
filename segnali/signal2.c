#include <signal.h>
#include <stdio.h>

/* sig_atomic_t is an integer type that is guaranteed by the standard to not be partially written or partially read in the presence of asynchronous interrupts
*/
static volatile sig_atomic_t doneflag = 0;

void setdoneflag(int signum){
	doneflag = 1;
}

int main(){
	struct sigaction act;
	
	// set up signal handler
	act.sa_handler = setdoneflag;
	act.sa_flags = 0;
	
	if((sigemptyset(&act.sa_mask) == -1) || (sigaction(SIGINT, &act, NULL) == -1)){
		perror("failed to set SIGINT handler");
		return 1;
	}
	
	while(!doneflag){
		printf("press CTR+C to kill the loop\n");
		sleep(1);
	}
	
	printf("program terminating...\n");
	return 0;
}
