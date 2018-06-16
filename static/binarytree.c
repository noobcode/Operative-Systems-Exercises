/*
 * ASSEGNAMENTO1-7:
 * Scrivere un programma che, dato un array di N elementi interi, costruisca un 
 * albero binario di ricerca (cioe' per ogni nodo dell'albero , l'elemento del nodo 
 * e' maggiore di tutti gli elementi del sottoalbero di sinistra e minore o uguale di 
 * tutti gli elementi del sottoalbero di destra). 
 * Implementare oltre alla funzione 'buildTree', le seguenti funzioni:
 *  - long getMin(struct node_t *root);   // restituisce il valore minimo
 *  - long getMax(struct node_t *root);   // restituisce il valore massimo
 *  - void printInOrder(struct node_t *root); // stampa gli elementi in modo ordinato
 *  - void deleteTree(struct node_t *root); // cancella tutti i nodi dell'albero
 *
 * COMPILAZIONE:
 *  gcc -std=c99 -Wall -g ./binarytree.c -o binarytree
 *
 */



#include <stdio.h>
#include <stdlib.h>
#include <mcheck.h>
#include "libtree.c"

int main(int argc, char *argv[]) {
    struct node_t *root = NULL; long i;
    const long array_size = 10;
    long array[10] = { 12, 32, 18, -1, 0, 18, -5, 54, 28, 15};
    mtrace();
    for(i=0; i<array_size; ++i)
	root = (struct node_t*)buildTree(array[i], root);
	
    printf("Min: %ld\n", (long)getMin(root));
    printf("Max: %ld\n", (long)getMax(root));

    printInOrder(root);
    printf("\n");
	
    deleteTree(root);
    muntrace();
    return 0;
}
