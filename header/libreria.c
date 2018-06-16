#include "libreria.h"
#include <stdio.h>
#include <stdlib.h>

// struttura node dell'albero
struct node_t {
    long data;
    struct node_t *left; 
    struct node_t *right;
};

// funzione di utilita' per controllo errori fatali
void checkFailure(void *p, const char str[]) {
    if (!p) { 
	perror(str);
	exit(EXIT_FAILURE); 
    }
}  

// costruisce un albero binario di ricerca 
struct node_t *buildTree(long elem, struct node_t *t) {
    if (t == NULL) { // albero vuoto
	checkFailure(t = malloc(sizeof(struct node_t)), "malloc");
	t ->data  = elem;	
	t ->left  = NULL;
	t ->right = NULL;
    } else {
	if (elem < t->data) // elemento minore della radice
	    t->left  = buildTree(elem, t->left); // percorro il sottoalbero sinistro 
	else 
	    t->right = buildTree(elem, t->right); // percorro il sottoalbero destro
    }
    return t;
}

// dealloco l'albero
void deleteTree(struct node_t *t) {
    if (t != NULL) {
	deleteTree(t->left);
	deleteTree(t->right);
	free(t);
    }
}

// si suppone che t != NULL, il controllo va fatto prima
// della chiamata a getMin
long getMin(struct node_t *t) {
    if (t->left==NULL)  // albero di sinistra vuoto
	return t->data;	// il minimo e' la radice
    else
	return getMin(t->left);	
}

// si suppone che t != NULL, il controllo va fatto prima
// della chiamata a getMax
long getMax(struct node_t *t) {
    if (t->right == NULL) // albero di destra vuoto
	return t->data;   // il massimo e' la radice
    else 
	return getMax(t->right);
}
// stampa i nodi dell'albero facendo una visita ordinata
void printInOrder(struct node_t *t) {
    if (t != NULL) {
	printInOrder(t->left);
	printf("%ld ", t->data);
	printInOrder(t->right);
    }
}
