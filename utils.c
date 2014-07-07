#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

/* Devido ao modo como tratamos as árvores, esse método facilita quando formos executar a remoção de arcos de uma árvore */
List *removeFromTree(List **tree, Vertex x, Vertex w){
	List *aux1, *aux2;
	List *aux;

	for(aux = tree[x]; next(aux) != NULL; aux = next(aux)){
		if(getVertexX(getArc(next(aux))) == x && getVertexW(getArc(next(aux))) == w){
			aux1 = removeNext(aux); 
		}
	}
	for(aux = tree[w]; next(aux) != NULL; aux = next(aux)){
		if(getVertexX(getArc(next(aux))) == x && getVertexW(getArc(next(aux))) == w){
			aux2 = removeNext(aux);
		}
	}
	if(isEqual(getArc(aux1), getArc(aux2))) return aux1;
	else {
		printf("Problema com manipulação da árvore. \n\n");
		return NULL;
	} 
}


/* Função recursiva que será usada para percorrermos as árvores soluções através de uma DFS, atualizando os Y's e alguns vetores auxiliares */
void dfsR(List **tree, Vertex x, int *prnt, int *dpth, int *ys, int *costVector){
	List *aux;
	Arc *arcAux;
	for(aux = tree[x]; next(aux) != NULL; aux = next(aux)){
		arcAux = getArc(next(aux));
		/* Se o início do arco é em x */
		if(getVertexX(arcAux) == x){
			if(dpth[getVertexW(arcAux)] == -1){
				prnt[getVertexW(arcAux)] = x; /* No arco X->W indica que X é pai de W */
				dpth[getVertexW(arcAux)] = dpth[x] + 1; /* Aumentamos a profundidade do vertice W baseado em seu pai X */
				ys[getVertexW(arcAux)] = ys[x] + costVector[getCost(arcAux)]; /* Como é um arco X->W, y[W] = y[X] + c(X->W) */
				dfsR(tree, getVertexW(arcAux), prnt, dpth, ys, costVector);
			}
		}
		/* Se o fim do arco é em x (arco contrário) */
		else {
			if(dpth[getVertexX(arcAux)] == -1){
				prnt[getVertexX(arcAux)] = x; /* Na árvore, o arco é W->X mas chegamos em W atraves do X, então X é pai de W */
				dpth[getVertexX(arcAux)] = dpth[x] + 1; /* Como X é pai de W, dpth[W] = dpth[X] + 1 */
				ys[getVertexX(arcAux)] = ys[x] - costVector[getCost(arcAux)]; /* como o arco é W->X e só sabemos o valor de y[X], então y[X] = y[W] + c(W->X) => y[W] = y[X] - c(W->X)*/
				dfsR(tree, getVertexX(arcAux), prnt, dpth, ys, costVector);
			}
		}
	}
}

/* Função para calcular os Y's da árvore recebida a partir dos vetores auxiliares costVector, 
   parent, depth e ys, alem da origem e número de vértices                                    */
void calculaYs(int *costVector, int *parent, int *depth, int *ys, int origem, int V, List **tree){
	Vertex aux;
	/* Inicializações para a DFS */
	for(aux = 0; aux < V; aux++){
		ys[aux] = depth[aux] = parent[aux] = -1;
	}
	parent[origem] = V;
	depth[origem] = 0;
	ys[origem] = 0;

	/* Primeiro chamamos a recursão da busca em profundidade para o 1o vértice (origem). Como
	   nós estamos percorrendo um grafo que já é uma árvore, nos com certeza formaremos uma árvore
	   no vetor "parent", sendo assim, só precisamos chamar a recursão explicitamente nesse momento,
	   pois o processo recursivo irá percorrer o resto da árvore automaticamente.                     */
	dfsR(tree, origem, parent, depth, ys, costVector);

}




/* Função que executa o método Simplex para redes */
List **resolveSimplex(List **tree, int *costVector, int origem, int destino, Digraph *G, int infinito){
	Vertex aux1;
	Vertex xAux, wAux;
	List *listAux;
	List *listAux1;
	Cycle *ciclo;
	Arc *arcAux;
	int alteracoes = 1;
	int *ys;
	int *parent;
	int *depth; 

	parent = malloc((G->V)*sizeof(int));
	depth = malloc((G->V)*sizeof(int));
	ys = malloc((G->V)*sizeof(int));
	ciclo = initCycle();


	/* Calculamos os Ys da primeira solução do simplex (obtida na 1a fase do algoritmo) para iniciarmos a 2a fase */
	calculaYs(costVector, parent, depth, ys, origem, G->V, tree);

	while(alteracoes != 0){
		alteracoes = 0;
		for(aux1 = 0; aux1 < G->V; aux1++){
			for(listAux = getAdj(G)[aux1]; next(listAux) != NULL;){
				/* Agora q possuimos os Y's atuais, se valer a pena, adcionaremos uma aresta atual e removeremos 
				   uma aresta desnecessária. Repetiremos esse processo até chegarmos à solução ótima (não há mais
				   inserções que melhorem a solução final)                                                        */
				arcAux = getArc(next(listAux));
				xAux = getVertexX(arcAux);
				wAux = getVertexW(arcAux);
				/* Esse if só é realmente útil quando estamos executando a 2a fase do simplex (o if trata da eliminação de arcos artificiais) */
				if(costVector[getCost(arcAux)] == infinito){
					listAux1 = removeNext(listAux);
					freeList(listAux1);
				}
				/* Como o arco está no grafo, vale que G->adj[i] contem os arcos da forma i->W com qualquer W != i */
				else if((ys[wAux]) > (ys[xAux] + costVector[getCost(arcAux)])){
					/* Removemos o arco do grafo (sem a árvore) e o inserimos na árvore */
					listAux1 = removeNext(listAux);
					arcAux = getArc(listAux1);
					insertArc(tree[xAux], arcAux);
					insertArc(tree[wAux], arcAux);
					alteracoes = 1;

					/* Após a inserção de um arco, formamos um ciclo dentro da nossa árvore. Para removê-lo, 
					   devemos procurar nesse ciclo qual dos arcos contrários à direção do ciclo deve sair   */
										

					/* Após removido um dos arcos do ciclo, voltamos a ter uma árvore, assim, recalcularemos os Ys */
					calculaYs(costVector, parent, depth, ys, origem, G->V, tree);

				}
				/* Arco não deve ser inserido, logo, apenas avançamos */
				else listAux = next(listAux);




			}
		}
	}

	return tree;
}