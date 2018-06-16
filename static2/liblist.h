#ifndef _LIB_LIST_
#define _LIB_LIST_

typedef struct elem_t{
	char* s;
	struct elem_t* next;
	struct elem_t* prev;
}elem_t;

typedef struct list_t{
	elem_t* head;
	elem_t* tail;
	
}list_t;


list_t* createList(); // crea lista vuota
int destroyList(list_t* l);
int insertList(list_t* l, const char* s);
void printList(list_t* l);
elem_t* getNode(char* s);
void checkFailure(void*,const char*);
int conta(list_t* l);
#endif
