#ifndef __ARC_H_INCLUDED__
#define __ARC_H_INCLUDED__

#define Vertex int

/* Os arcos conterão a informação sobre o custo e o fluxo de um arco que vai do vértice X ao vértice W.
   Vale dizer que "indCost" na verdade é um índice que será usado para acessar uma posição em um 
   vetor de custos específico, a qual conterá o custo do arco em questão.                                */
typedef struct arc{
	Vertex x;
	Vertex w;
	int indCost;
	int flow;
} Arc;

Arc *newArc(Vertex x, Vertex w, int custo, int fluxo);

int getFlow(Arc *a);

int getCost(Arc *a);

Vertex getVertexX(Arc *a);

Vertex getVertexW(Arc *a);

void setFlow(Arc *a, int fluxo);

void freeArc(Arc *a);


#endif /* __ARC_H_INCLUDED__*/