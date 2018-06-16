#if !defined(LISTA_H_)
#define LISTA_H_
typedef struct elem {
  char *key;
  struct elem * next;
  struct elem * prev;
} elem_t;
 
typedef int (*compare_t)(const char *, const char *);

typedef struct {
    /** la testa della lista */
    elem_t * head;
    /** questa e' la funzione per confrontare due chiavi */
    compare_t compare;
} list_t;


#define CHECK_EQ(X, val, str)					      \
    if ((X)==val) {						      \
	perror(#str);						      \
	printf("ERRORE ALLA LINEA %d FILE %s\n", __LINE__, __FILE__); \
	exit(EXIT_FAILURE);					      \
    }
#define CHECK_NEQ(X, val, str)					      \
    if ((X)!=val) {						      \
	perror(#str);						      \
	printf("ERRORE ALLA LINEA %d FILE %s\n", __LINE__, __FILE__); \
	exit(EXIT_FAILURE);					      \
    }


list_t *createList();
int     destroyList(list_t *L);
int     insertList(list_t *L, const char *);
void    printList(list_t *L);

#endif /* LISTA_H_ */
