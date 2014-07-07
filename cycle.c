#include <stdlib.h>

#include "cycle.h"

/******************** Implementação das Funções *********************************/


/* Retorna um ciclo criado (estamos mexendo com listas ligadas com cabeça) */
Cycle *initCycle(){
	Cycle *new;
	new = malloc(sizeof(Cycle));
	new->next = NULL;
	return new;
}


/* Retorna o prox elemento do ciclo depois de A(podendo A ser a cabeça) */
Cycle *nextOnCycle(Cycle *A){
	return A->next;
}

/* Remove o elemento após o elemento passado como parâmetro */ 
Cycle *removeNextOnCycle(Cycle *A){
	Cycle *aux;
	aux = A->next;
	A->next = aux->next;
	return aux; 
}

/* Retorna o ponteiro para um arco que existe na célula do ciclo */
Arc *getArcFromCycle(Cycle *A){
	return A->arc;
}

/* Recebe um ciclo e 2 vertices x e w e verifica se no ciclo existe o arco x->w e o retorna */
Arc *hasArcOnCycle(Cycle *A, Vertex x, Vertex w){
	Cycle *aux;
	for(aux = nextOnCycle(A); aux != NULL; aux = nextOnCycle(aux) ){
		if(getVertexX(getArcFromCycle(aux)) == x && getVertexW(getArcFromCycle(aux)) == w) return getArcFromCycle(aux);
	}
	return NULL;
}


/* Insere um arco no ciclo na primeira posição (após a cabeça A) */
void insertArcOnCycle(Cycle *A, Arc *arc, int direction){
	Cycle *new;
	new = malloc(sizeof(Cycle));
	new->arc = arc;
	new->direction = direction;
	new->next = A->next;
	A->next = new;

}

/* Apagamos o ciclo, liberando a memória consumida por ele  */
void freeCycle(Cycle *A){
	Cycle *aux1, *aux2;
	for(aux1 = A, aux2 = A->next; aux2 != NULL; aux1 = aux2, aux2 = aux1->next){
		freeArc(getArcFromCycle(aux1));
		free(aux1);
	}
	freeArc(getArcFromCycle(aux1));
	free(aux1);
}

