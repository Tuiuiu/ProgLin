#ifndef __DIGRAPH_H_INCLUDED__
#define __DIGRAPH_H_INCLUDED__

#include "fwdList.h"


typedef struct digraph{
	int V;
	int A;
	List **adj;
} Digraph;

Digraph *initDigraph(int nV, int nA, List **adjacentes);

List **getAdj(Digraph *G);

void freeDigraph(Digraph *G);


#endif /* __DIGRAPH_H_INCLUDED__*/