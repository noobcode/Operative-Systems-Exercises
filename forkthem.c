#include <stdio.h>
#include <unistd.h>

void forkthem(int n){
	if(n > 0){
		fork();
		forkthem(n-1);
	}
}

int main(){
	forkthem(2);
	printf("%d --> %d\n", (int)getppid(), (int)getpid());
	return 0;
}


