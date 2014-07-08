#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

/* Devido ao modo como tratamos as árvores, esse método facilita quando formos executar a remoção de arcos de uma árvore */
List *removeFromTree(List **tree, Vertex x, Vertex w){
	List *aux1, *aux2;
	List *auxA, *auxB;

	for(auxA = tree[x]; next(auxA) != NULL; auxA = next(auxA)){
		if(getVertexX(getArc(next(auxA))) == x && getVertexW(getArc(next(auxA))) == w){
			aux1 = removeNext(auxA); 
			break;
		}
	}
	for(auxB = tree[w]; next(auxB) != NULL; auxB = next(auxB)){
		if(getVertexX(getArc(next(auxB))) == x && getVertexW(getArc(next(auxB))) == w){
			aux2 = removeNext(auxB);
			break;
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
List **resolveSimplex(List **tree, int *costVector, int origem, int destino, Digraph *G, int infinito, int qtdProduto){
	Vertex aux1, aux2;
	Vertex xAux, wAux;
	List *listAux;
	List *listAux1, *listAux2;
	Cycle *ciclo;
	Cycle *cicloAux;
	Cycle *cicloAux1;
	Arc *arcAux;
	Arc *arcoASair;
	int delta = qtdProduto+1;
	int alteracoes = 1;
	int *ys;
	int *parent;
	int *depth; 

	parent = malloc((G->V)*sizeof(int));
	depth = malloc((G->V)*sizeof(int));
	ys = malloc((G->V)*sizeof(int));

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
					ciclo = initCycle();
					insertArcOnCycle(ciclo, arcAux, 1);


					/* Vamos primeiramente igualar os niveis de "wAux" e "xAux", inserindo arcos no ciclo durante esse processo */
					if(depth[xAux] > depth[wAux]){
						while(depth[xAux] > depth[wAux]){
							aux2 = parent[xAux];
							if((arcAux = hasArc(tree[xAux], xAux, aux2)) != NULL){
								insertArcOnCycle(ciclo, arcAux, 0);
								if(getFlow(arcAux) < delta){
									delta = getFlow(arcAux);
									arcoASair = arcAux;
								}
							}
							else if((arcAux = hasArc(tree[aux2], aux2, xAux)) != NULL){
								insertArcOnCycle(ciclo, arcAux, 1);
							}
							/* Pelo jeito como depth é preenchido, depth[parent[xAux]] = depth[xAux]-1, logo */
							xAux = aux2;
						}
					}	
					else if(depth[xAux] < depth[wAux]){
						while(depth[xAux] < depth[wAux]){
							aux2 = parent[wAux];
							if((arcAux = hasArc(tree[wAux], wAux, aux2)) != NULL){
								insertArcOnCycle(ciclo, arcAux, 1);
							}
							else if((arcAux = hasArc(tree[aux2], aux2, wAux)) != NULL){
								insertArcOnCycle(ciclo, arcAux, 0);
								if(getFlow(arcAux) < delta){
									delta = getFlow(arcAux);
									arcoASair = arcAux;
								}
							}
							/* Pelo jeito como depth é preenchido, depth[parent[wAux]] = depth[wAux]-1, logo */
							wAux = aux2;
						}
					}

					/* Agora, com os depth's igualados, vamos subir os 2 lados do ciclo juntamente, 
					   até que cheguemos em um mesmo vértice, completando o ciclo                    */
					if(depth[xAux] == depth[wAux]){
						while(xAux != wAux){
							/* Repetimos o que foi feito anteriormente com xAux */
							aux2 = parent[xAux];
							if((arcAux = hasArc(tree[xAux], xAux, aux2)) != NULL){
								insertArcOnCycle(ciclo, arcAux, 0);
								if(getFlow(arcAux) < delta){
									delta = getFlow(arcAux);
									arcoASair = arcAux;
								}
							}
							else if((arcAux = hasArc(tree[aux2], aux2, xAux)) != NULL){
								insertArcOnCycle(ciclo, arcAux, 1);
							}
							xAux = aux2;

							/* Repetimos o que foi feito anteriormente, mas agora com o wAux */
							aux2 = parent[wAux];
							if((arcAux = hasArc(tree[wAux], wAux, aux2)) != NULL){
								insertArcOnCycle(ciclo, arcAux, 1);
							}
							else if((arcAux = hasArc(tree[aux2], aux2, wAux)) != NULL){
								insertArcOnCycle(ciclo, arcAux, 0);
								if(getFlow(arcAux) < delta){
									delta = getFlow(arcAux);
									arcoASair = arcAux;
								}
							}
							wAux = aux2;

							/* O laço acima ocorrerá até que parent[xAux] = parent[wAux],  
							   e quando isso acontecer, teremos no inicio da próxima execução
							   que xAux = wAux. Assim, o while parará.                         */
						} 
					}
					/* Agora que encontramos o ciclo, obtivemos também o "delta" e o arco que sairá do ciclo 
					   (sai o arco que mais restringe a alteração no fluxo)                                  
					   Sendo assim, temos :                                                                   */
					for(cicloAux = ciclo, cicloAux1 = nextOnCycle(cicloAux); cicloAux1 != NULL; ){
						/* Arcos contra o ciclo. Aqui nós decrementamos seu fluxo pelo valor de delta e,
						   caso o arco analisado seja o que deve sair, o retiramos da arvore e o 
						   inserimos de volta ao grafo.                                                  */
						if(getDirection(cicloAux1) == 0){
							arcAux = getArcFromCycle(cicloAux1);
							setFlow(arcAux, getFlow(arcAux)-delta);
							if(isEqual(arcAux, arcoASair)){
								/* Removo da árvore o arco X-W */
								listAux2 = removeFromTree(tree, getVertexX(arcAux), getVertexW(arcAux));
								/* E o insiro no Grafo (que contém todos arcos fora da árvore) */
								insertArc(getAdj(G)[getVertexX(arcAux)], arcAux);
							}
							cicloAux = cicloAux1;
							cicloAux1 = nextOnCycle(cicloAux);
						}
						else{ /* getDirection(cicloAux1) == 1 */
						/* Arcos a favor do ciclo. Apenas incrementamos os fluxos pelo valor de delta */
							arcAux = getArcFromCycle(cicloAux1);
							setFlow(arcAux, getFlow(arcAux) + delta);
							cicloAux = cicloAux1;
							cicloAux1 = nextOnCycle(cicloAux);
						}
					}
					/* Liberamos os ponteiros do ciclo */
					freeCycle(ciclo);
					/* Após removido um dos arcos do ciclo, voltamos a ter uma árvore, assim, recalcularemos os Ys */
					calculaYs(costVector, parent, depth, ys, origem, G->V, tree);
					/* Termina aqui o caso onde inserimos um arco novo na árvore e retiramos um dos que formam o ciclo */
				}
				/* Arco não deve ser inserido, logo, apenas avançamos */
				else { listAux = next(listAux); }
			}
		}
		/* Ao fim desse laço, teremos percorrido o grafo todo, inserindo arcos pertinentes e 
		   removendo arcos nos casos onde formamos ciclos. Caso algum arco tenha sido inserido (tenha
		   ocorrido alteração na árvore), nossa variavel "alteracao" passa a valer 1, sinalizando que o 
		   simplex deverá percorrer novamente o grafo em busca de arestas a serem inseridas. Quando o 
		   algoritmo percorrer o grafo todo e nenhuma aresta for inserida, terminamos o loop e na árvore
		   "tree" estará contida a árvore com a solução final ótima para o problema.                      */
	}

	return tree;
}