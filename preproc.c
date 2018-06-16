#include<stdio.h>
#define N 10

int max = 0;

int main(){
	int i, tmp;
	printf("inserisci %d interi positivi\n", N);
	for(i = 0; i < N; i++){
		scanf("%d",&tmp);
		max = (max > tmp) ? max : tmp; 	
	}
	printf("il massimo è %d\n", max);
	return 0;
}
