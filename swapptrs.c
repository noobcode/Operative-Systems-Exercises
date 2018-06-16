/*
 * ASSEGNAMENTO1-1:
 *  Scrivere una funzione swapPtr che swappa due puntatori qualsiasi. 
 * 
 * COMPILAZIONE:
 *  gcc -std=c99 -Wall -g ./swapptrs.c -o swapptrs
 *
 */


#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>

void swapPtr(void **ptr1, void **ptr2) {
    // è buona norma usare il più possibile 'assert' 
    // per testare pre-condizioni, 
    // le assert vengono rimosse automaticamente dal 
    // compilatore se il programma viene compilato definendo NDEBUG 
    // (cioè: -DNDEBUG).
    assert(ptr1); assert(ptr2); 
    void *tmp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = tmp;
}

int main() {
    long a = 11, b = 22;
    void *ptr1 = &a;
    void *ptr2 = &b;

    printf("PRIMA DELLO SWAP: ptr1=%p (*ptr1=%ld)  ptr2=%p (*ptr2=%ld)\n", ptr1, *(long*)ptr1, ptr2, *(long*)ptr2);

    swapPtr(&ptr1, &ptr2);

    printf("\nDOPO LO SWAP: ptr1=%p (*ptr1=%ld)  ptr2=%p (*ptr2=%ld)\n",  ptr1, *(long*)ptr1, ptr2, *(long*)ptr2);

    return 0;
}
