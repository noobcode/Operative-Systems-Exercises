/*
 * ASSEGNAMENTO1-8:
 *  Scrivere un programma che dato un intero A a 32bit calcoli lo xor
 *  dei 2 byte piu' significativi (quelli di sinistra) con i 2 byte 
 *  meno significativi.
 *  Dato A1 in formato network-byte-order di A (usare A1=htonl(A), 
 *  vedi man 3 htonl), riordinare i byte di A1 in host-byte-order. 
 *     
 * COMPILAZIONE:
 *  gcc -std=c99 -Wall -g ./intxor.c -o intxor
 *
 */

#include <stdint.h>
#include <stdio.h>
#include <arpa/inet.h> // necessario per htonl

int main() {
    
    int32_t A = 0x1234567; // codifica esadecimale di 19088743
    // stampo i singoli byte di A in notazione esadecimale
    // dal byte meno significativo al byte piu' significativo
    printf("a1=0x%x, a2=0x%x, a3=0x%x, a4=0x%x\n", 
	   A & 0xFF, (A>>8) & 0xFF, (A>>16) & 0xFF, (A>>24) & 0xFF);
    printf("A=%d (=0x%x)\n", A,A);
    {  // primo modo
	int16_t B = (A >> 16);    // prendo i 2 byte piu' signicativi
	int16_t C = (A & 0xFFFF); // prendo i 2 byte meno significativi	
	printf("B=%d, C=%d\n", B, C);	
	printf("B^C=%d\n", B^C); // xor utilizzando l'operatore ^	
	printf("B^C=%d\n", ((B & ~C) | (~B & C))); // xor utilizzando  & ~ |
    }
    {  // altro modo
	union {
	    struct stB {
		// è possibile usare anche int16_t
		int32_t C:16;  // bytes meno significativi
		int32_t B:16;  // bytes più significativi
	    } K;
	    int32_t K2;
	} U;
	U.K2 =  A; // memorizzo A in K
	printf("B=%d, C=%d\n", U.K.B, U.K.C);
	printf("B^C=%d\n", U.K.B ^ U.K.C); 
    }
    // seconda parte
    {
	int32_t A1 = htonl(A); // converto A in network-byte-order (big-endian)
	union {
	    struct stA1 {
		// è possibile usare anche int8_t
		int32_t a1:8; // byte meno significativo
		int32_t a2:8;
		int32_t a3:8;
		int32_t a4:8; // byte piu' significativo
	    } Y;
	    int32_t Y2;
	} U;
	U.Y2 = A1; // memorizzo A1 in Y
	// stampo i singoli byte in notazione esadecimale
	printf("a1=0x%x, a2=0x%x, a3=0x%x, a4=0x%x\n", U.Y.a1,U.Y.a2,U.Y.a3,U.Y.a4);
	// ricostruisco A riordinando i byte in formato host-byte-order (little-endian)
	printf("A=%d\n", (U.Y.a4|(U.Y.a3<<8)|(U.Y.a2<<16)|(U.Y.a1<<24)));

#if 0
	//altro modo di procedere	
	U.Y2 = A1;
	int8_t *p = (int8_t*)&U.Y2;
	int8_t a1 = *p++;
	int8_t a2 = *p++;
	int8_t a3 = *p++;
	int8_t a4 = *p;
	U.Y.a1 = a4;
	U.Y.a2 = a3;
	U.Y.a3 = a2;
	U.Y.a4 = a1;	    
	printf("A=%d\n", U.Y2);
#endif

    }
    
    return 0;
}
