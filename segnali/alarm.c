#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main(){
	alarm(3);
	printf("inizio ciclo infinito\n");
	while(1);
	printf("pippo\n"); // mai eseguita
	return 0;
}
