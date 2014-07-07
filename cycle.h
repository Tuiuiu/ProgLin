#ifndef __CYCLE_H_INCLUDED__
#define __CYCLE_H_INCLUDED__

#include "arc.h"

/* A estrutura "Cycle" funciona de modo extremamente semelhante a "List", a única diferença é a 
   presença de um campo "direction", ou seja, quando formarmos um ciclo, queremos saber se o arco 
   presente na célula do ciclo em questão é à favor ou contra a direção do ciclo (util na execução do simplex). */
typedef struct cycleList{
	Arc *arc;
	int direction;
	struct cycleList *next;
} Cycle;



Cycle *initCycle();

Cycle *nextOnCycle(Cycle *A);

Cycle *removeNextOnCycle(Cycle *A);

Arc *getArcFromCycle(Cycle *A);

Arc *hasArcOnCycle(Cycle *A, Vertex x, Vertex y);

void insertArcOnCycle(Cycle *A, Arc *arc, int direction);

void freeCycle(Cycle *A);

#endif /* __CYCLE_H_INCLUDED__*/