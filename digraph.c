#include <stdlib.h>

#include "digraph.h"

Digraph *initDigraph(int nV, int nA, List **adjacentes){
	Digraph *G;
	G = malloc(sizeof(Digraph));
	G->V = nV;
	G->A = nA;
	G->adj = adjacentes;
	return G;
}

List **getAdj(Digraph *G){
	return G->adj;
}

void freeDigraph(Digraph *G){
	int i;
	for(i = 0; i < G->V; i++)
		freeList(G->adj[i]);
	free(G);
}