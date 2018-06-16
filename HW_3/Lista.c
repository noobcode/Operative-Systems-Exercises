#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <Lista.h>

// strdup non e' C99, e' necessario il prototipo
// altrimenti non viene riconosciuta dal compilatore
char *strdup(const char *s);

// NOTA:
// alternativa con diversa interfaccia per la createList
//    list_t *createList(compare_t cmp)
// in questo caso andava controllato che cmp != NULL e quindi 
// L->compare = cmp;
list_t *createList() {
    list_t *L = (list_t*)malloc(sizeof(list_t));
    if (L == NULL) {
	perror("createList:malloc");
	return NULL;
    }
    elem_t *p1 = malloc(sizeof(elem_t));
    if (p1 == NULL) {
	perror("createList:malloc");
	return NULL;
    }
    p1->key    = NULL;
    p1->next   = p1->prev = NULL;
    L->head    = p1;
    L->compare = strcmp;

    return L;
}

int  insertList(list_t *L, const char *str) {
    assert(str);

    elem_t *p1 = malloc(sizeof(elem_t));
    if (p1 == NULL) return -1;

    p1->key = strdup(str);

    int r=1;
    elem_t *p = L->head;
    while(p->key && (r=L->compare(p->key, str))<=0) p=p->next;    
    p1->next = p;
    p1->prev = p->prev;
    // potri essere al primo elemento
    if (p->prev) p->prev->next = p1;
    p->prev = p1;
    if (L->head == p) L->head = p1;

    return 0;
}
void printList(list_t *L) {
    long count = 0;
    elem_t *p = L->head;
    while(p->key) {
	printf("%ld:\t%s\n",count++, p->key);
	p=p->next;
    }
}
int  destroyList(list_t *L) {
    elem_t *p = L->head, *t = NULL;
    while(p->key) {
	t = p->next;
	free(p->key);
	free(p);
	p=t;
    }
    free(p);
    free(L);
    return 0;
}
