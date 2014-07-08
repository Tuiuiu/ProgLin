#include <stdlib.h>

#include "arc.h"


/* Construtor do arco */
Arc *newArc(Vertex x, Vertex w, int custo, int fluxo){
	Arc *new;
	new = malloc(sizeof(Arc));
	new->x = x;
	new->w = w;
	new->indCost = custo;
	new->flow = fluxo;
	return new;
}

/* Retorna o fluxo do arco passado como parametro */
int getFlow(Arc *a){
	return a->flow;
}

/* Retorna o indice de custo do arco passado como parametro */
int getCost(Arc *a){
	return a->indCost;
}

/* Retorna o vertice X do arco (o inicio do arco) */
Vertex getVertexX(Arc *a){
	return a->x;
}

/* Retorna o vertice W do arco (o fim do arco) */
Vertex getVertexW(Arc *a){
	return a->w;
}

/* Seta o fluxo do arco de acordo com os parâmetros passados */
void setFlow(Arc *a, int fluxo){
	a->flow = fluxo;
}

/* Libera a memória utilizada pelo arco */
void freeArc(Arc *a){
	free(a);
}