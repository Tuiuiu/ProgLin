#ifndef __FWDLIST_H_INCLUDED__
#define __FWDLIST_H_INCLUDED__

#include "arc.h"

/* Nosso "struct list" é uma lista de células, onde cada célula possui os campos arc e next. 
   arc é um ponteiro para um objeto Arc e next é um ponteiro para o proximo elemento da lista. */
typedef struct list {
	Arc *arc;
	struct list *next;
} List;



List *initList();

List *next(List *A);

List *removeNext(List *A);

Arc *getArc(List *A);

Arc *hasArc(List *A, Vertex x, Vertex y);

int isEqual(Arc *a, Arc *b);

void insertArc(List *A, Arc *arc);

void freeList(List *A);

#endif /* __FWDLIST_H_INCLUDED__*/ 