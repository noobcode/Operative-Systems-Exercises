#include <stdio.h>
#include <arpa/inet.h>

int main(){
	uint16_t n, nnet, nhost;
	unsigned char* p;
	
	n = 0xd04c;
	p = (unsigned char*)&n;
	printf("%x %x\n", *p, *(p+1)); // host byte order
	nnet = htons(n);
	p = (unsigned char*)&nnet;
	printf("%x %x\n", *p, *(p+1)); // network byte order
	nhost = ntohs(nnet);
	p = (unsigned char*)&nhost;	
	printf("%x %x\n", *p, *(p+1)); // host byte order
	return 0;
}
