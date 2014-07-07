#ifndef __FWDLIST_H_INCLUDED__
#define __FWDLIST_H_INCLUDED__

#include "arc.h"

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