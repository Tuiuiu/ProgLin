#include <stdlib.h>

#include "digraph.h"

/* Construtor do dígrafo */
Digraph *initDigraph(int nV, int nA, List **adjacentes){
	Digraph *G;
	G = malloc(sizeof(Digraph));
	G->V = nV;
	G->A = nA;
	G->adj = adjacentes;
	return G;
}

/* Retorna o vetor de listas de adjacencia do dígrafo */
List **getAdj(Digraph *G){
	return G->adj;
}

/* Libera a memoria utilizada pelo nosso dígrafo */
void freeDigraph(Digraph *G){
	int i;
	for(i = 0; i < G->V; i++)
		freeList(G->adj[i]);
	free(G);
}