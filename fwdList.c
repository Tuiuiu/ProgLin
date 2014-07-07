/* Lista de includes */
#include <stdlib.h>

#include "fwdList.h"


/******************** Implementação das Funções *********************************/


/* Retorna uma lista criada (estamos mexendo com listas ligadas com cabeça) */
List *initList(){
	List *new;
	new = malloc(sizeof(List));
	new->next = NULL;
	return new;
}


/* Retorna o prox elemento da lista depois de A(podendo A ser a cabeça) */
List *next(List *A){
	return A->next;
}

/* Remove o elemento após o elemento passado como parâmetro */ 
List *removeNext(List *A){
	List *aux;
	aux = A->next;
	A->next = aux->next;
	aux->next = NULL;
	return aux; 
}

/* Retorna o ponteiro para um arco que existe na célula da lista */
Arc *getArc(List *A){
	return A->arc;
}

/* Recebe uma lista e 2 vertices x e w e verifica se na lista existe o arco x->w e o retorna */
Arc *hasArc(List *A, Vertex x, Vertex w){
	List *aux;
	for(aux = next(A); aux != NULL; aux = next(aux) ){
		if(getVertexX(getArc(aux)) == x && getVertexW(getArc(aux)) == w) return getArc(aux);
	}
	return NULL;
}

/* Recebe dois arcos e verifica se são o mesmo */
int isEqual(Arc *a, Arc *b){
	if (getVertexX(a) == getVertexX(b) &&
		getVertexW(a) == getVertexW(b) &&
		getFlow(a) == getFlow(b) &&
		getCost(a) == getCost(b)) return 1;
	else return 0;
}

/* Insere um arco na lista na primeira posição (após a cabeça A) */
void insertArc(List *A, Arc *arc){
	List *new;
	new = malloc(sizeof(List));
	new->arc = arc;
	new->next = A->next;
	A->next = new;

}

/* Apagamos a lista, liberando a memória consumida por ela  */
void freeList(List *A){
	List *aux1, *aux2;
	for(aux1 = A, aux2 = A->next; aux2 != NULL; aux1 = aux2, aux2 = aux1->next){
		freeArc(getArc(aux1));
		free(aux1);
	}
	freeArc(getArc(aux1));
	free(aux1);
}

