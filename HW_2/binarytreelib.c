#include <stdlib.h>
#include <stdio.h>
#include <binarytree.h>


// costruisce un albero binario di ricerca 
struct node_t *buildTree(long elem, struct node_t *t) {
    if (t == NULL) { // albero vuoto
	if ((t = malloc(sizeof(struct node_t)), "malloc") == NULL) {
	    perror("malloc");
	    exit(EXIT_FAILURE);
	}
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
