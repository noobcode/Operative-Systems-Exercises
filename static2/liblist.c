#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "liblist.h"
#define LEN 101

list_t* createList(){
	list_t* l = malloc(sizeof(list_t));
	elem_t* el = malloc(sizeof(elem_t));
	el->s = NULL;
	el->next = el->prev = NULL;
	l->head = el;
	l->tail = el;
	
	return l;
}

int destroyList(list_t* l){
	elem_t* curr = l->head;
	elem_t* next;
	while(curr){
		next = curr->next;
		free(curr->s);
		free(curr);
		curr = next;
	}
	free(l);
	return 0;
}

int insertList(list_t* l, const char* str){
	assert(str);
	char *s1 = strdup(str);
	elem_t* curr = l->head;
	if(curr->s == NULL) 
		curr->s = s1;
	else{
		elem_t* el = getNode(s1);
		checkFailure(el, "getNode");
		
		while(curr && (strcmp(curr->s, el->s) <= 0))
			curr = curr->next;
		if(!curr){	// inserisci in coda
			l->tail->next = el;
			el->prev = l->tail;
			l->tail = el;
		}else{	// inserisci in mezzo
			el->next = curr;
			if(curr->prev){
				curr->prev->next = el;
				el->prev = curr->prev;
			}else{
				l->head = el;
			}
			curr->prev = el;
		}
	}
	return 0;
}

void printList(list_t* l){
	elem_t* curr = l->head;
	while(curr){
		printf("%s\n",curr->s);
		curr = curr->next;
	}
}

elem_t* getNode(char* str){
	elem_t* el = malloc(sizeof(elem_t));
	el->s = malloc(LEN * sizeof(char));
	el->s = str;
	el->next = el->prev = NULL;
	return el;
}

void checkFailure(void* p, const char * s){
	if(!p){
		perror(s);
		exit(EXIT_FAILURE);
	}
}

int conta(list_t* l){
	int count = 0;
	elem_t* curr = l->head;
	while(curr){
		count++;
		curr = curr->next;
	}
	return count;		
}
