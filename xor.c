#include<stdio.h>
#include<stdint.h>
#include<arpa/inet.h>

typedef struct number{
	int8_t first;
	int8_t second;
}number;

int main(){
	int16_t A = 1557;
	int16_t B;
	number a, b;
	a.first = A>>8; 
	a.second = A & 0x00FF;
	printf("A = %d, (0x%x)\n",A,A);
	printf("a.first %d (0x%x)\na.second %d (0x%x)\n", a.first, a.first, a.second, a.second);
	printf("a.first XOR a.second %d (0x%x)\n", (a.first ^ a.second), (a.first ^ a.second));
	
	B = htons((uint16_t)A); // converte A in formato network byte order 
	printf("A in formato network-byte-order %d (0x%x)\n", B, B);
	b.first = B>>8;
	b.second = B & 0x00FF;
	printf("b.first %d (0x%x)  b.second %d (0x%x)\n", b.first, b.first, b.second, b.second);
	B = ((int16_t)b.second)<<8 | (int16_t)b.first;
	printf("%d, (0x%d)\n", B, B); 
	
	return 0;
}
