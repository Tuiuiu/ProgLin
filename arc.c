#include <stdlib.h>

#include "arc.h"


Arc *newArc(Vertex x, Vertex w, int custo, int fluxo){
	Arc *new;
	new = malloc(sizeof(Arc));
	new->x = x;
	new->w = w;
	new->indCost = custo;
	new->flow = fluxo;
	return new;
}

int getFlow(Arc *a){
	return a->flow;
}

int getCost(Arc *a){
	return a->indCost;
}

Vertex getVertexX(Arc *a){
	return a->x;
}

Vertex getVertexW(Arc *a){
	return a->w;
}


void setFlow(Arc *a, int fluxo){
	a->flow = fluxo;
}

void freeArc(Arc *a){
	free(a);
}