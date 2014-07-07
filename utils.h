/* Headers */
#include "fwdList.h"
#include "cycle.h"
#include "arc.h"
#include "digraph.h"
	

/* Na biblioteca "utils" guardaremos funções auxiliares para a execução do programa */


List *removeFromTree(List **tree, Vertex x, Vertex w);

void dfsR(List **tree, Vertex x, int *prnt, int *dpth, int *ys, int *costVector);

List **resolveSimplex(List **tree, int *costVector, int origem, int destino, Digraph *G, int infinito);

void calculaYs(int *costVector, int *parent, int *depth, int *ys, int origem, int V, List **tree);