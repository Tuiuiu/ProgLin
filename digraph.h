#ifndef __DIGRAPH_H_INCLUDED__
#define __DIGRAPH_H_INCLUDED__

#include "fwdList.h"


/* Nosso digrafo possuirá um vetor de Listas de adjacência. Para cada posição i do vetor, 
   teremos os arcos com vértice inicial i (arcos i->w com qualquer w != i )               
   Também terá V (número de vértices do grafo) e A (número de arestas do grafo).          */
typedef struct digraph{
	int V;
	int A;
	List **adj;
} Digraph;



Digraph *initDigraph(int nV, int nA, List **adjacentes);

List **getAdj(Digraph *G);

void freeDigraph(Digraph *G);


#endif /* __DIGRAPH_H_INCLUDED__*/