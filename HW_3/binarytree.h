#if !defined(BINRAY_TREE_H)
#define BINARY_TREE_H

// struttura node dell'albero
struct node_t {
    long data;
    struct node_t *left; 
    struct node_t *right;
};

// costruisce un albero binario di ricerca 
struct node_t *buildTree(long elem, struct node_t *t);

// dealloco l'albero
void deleteTree(struct node_t *t);

// si suppone che t != NULL, il controllo va fatto prima
// della chiamata a getMin
long getMin(struct node_t *t);

// si suppone che t != NULL, il controllo va fatto prima
// della chiamata a getMax
long getMax(struct node_t *t);

// stampa i nodi dell'albero facendo una visita ordinata
void printInOrder(struct node_t *t);

// stampa i nodi dell'albero facendo una visita ordinata
void printInOrder(struct node_t *t);


#endif /* BINRARY_TREE_H */
