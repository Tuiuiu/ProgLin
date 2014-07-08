/*******************************************************************
   Nome : Lucas Dário
   No USP : 7990940
*******************************************************************

Para rodar o EP, basta utilizar o comando "make" no terminal, o arquivo
do makefile cuidará da compilação.
Para execução, " ./EP < <nome do arquivo de entrada>  " imprimirá
a resposta no terminal.

*******************************************************************/




/* Bibliotecas */
#include <stdio.h>
#include <stdlib.h>

/* Headers */
#include "utils.h"


int main(){

	/* Declarações de variáveis */
	int V;
	int A;
	int qtdProduto;
	int i;
	Vertex origem;
	Vertex destino;
	List *aux;
	List *aux2;
	Vertex auxX;
	Vertex auxY;
	int auxCost;
	int infinito = 0;
	Vertex vInicial;
	Digraph *G;
	List **adjacent;
	List **tree;
	List **solucaoInicial;
	List **solucaoFinal;
	Arc *arcAux;
	Arc *arcAux1;
	Arc *arcAux2;
	int *custosReais;
	int *custosArtificiais;
	int intAux;
	int custoFinal;
	/***************************/


	/* Inicialização - recebendo a entrada */

	A = 0;
	scanf("%d", &V);
	adjacent = malloc(V*sizeof(List*));
	for(i = 0; i < V; i++)
		adjacent[i] = initList();
	custosReais = malloc(V*sizeof(int));
	custosArtificiais = malloc(V*sizeof(int));
	intAux = V;
	scanf("%d", &origem);
	scanf("%d", &destino);
	scanf("%d", &qtdProduto);
	while(scanf("%d %d %d", &auxX, &auxY, &auxCost)!= EOF){
		arcAux1 = newArc(auxX, auxY, A, 0);
		insertArc(adjacent[auxX], arcAux1);
		/*insertArc(adj[auxY], arcAux1);*/
		infinito += auxCost;
		if(A == intAux){
			intAux *= 2;
			custosReais = realloc(custosReais, intAux*sizeof(int));
			custosArtificiais = realloc(custosArtificiais, intAux*sizeof(int));
		}
		custosReais[A] = auxCost;
		custosArtificiais[A] = 0;
		A++;
	}
	G = initDigraph(V, A, adjacent);

	/* Vamos selecionar um vertice "vInicial" (diferente da origem e do destino) para construirmos a árvore inicial */
	while(1){
		vInicial = rand() % V;
		if(vInicial != origem && vInicial != destino)
			break;
	}

	/* Alocamos a árvore*/
	tree = malloc(V*sizeof(List*));
	for(i = 0; i < V; i++)
		tree[i] = initList();

	/* Queremos que haja pelo menos 1 posição vazia em cada um dos vetores de custos. Vamos garantir isso aqui.
	   Sabemos que intAux não pode ser menor que A(pois isso é garantido durante a inicialização e preenchimento 
	   dos vetores de custo). Se for maior que A, então há espaço, c.c. alocamos 1 espaço a mais para cada um.   */
	if(intAux == A){
		intAux++;
		custosReais = realloc(custosReais, intAux);
		custosArtificiais = realloc(custosArtificiais, intAux);
	}

	/* As duas linhas seguintes serão utilizadas nos casos em que precisarmos de arcos artificiais */
	custosReais[A] = infinito+1;
	custosArtificiais[A] = 1;

	/* Preenchemos a árvore a partir do vértice inicial */
	for(i = 0; i < V; i++){
		if(i != vInicial){
			/* Se existir arco "i->vInicial" com i != destino, adcionamos à árvore inicial */
			if( (i != destino) && ((arcAux = hasArc(getAdj(G)[i], i, vInicial)) != NULL)){
				/* 'For' executado apenas para posicionar "aux" */
				for (aux = getAdj(G)[i], aux2 = next(aux); !isEqual(getArc(aux2), arcAux); aux = aux2, aux2 = next(aux));
				arcAux2 = getArc(removeNext(aux));
				insertArc(tree[i], arcAux2);
				insertArc(tree[vInicial], arcAux2);
			}
			/* Se existir arco "vInicial->i" com i != origem, adcionamos à árvore inicial */
			else if( (i != origem) && ((arcAux = hasArc(getAdj(G)[vInicial], vInicial, i)) != NULL)){
				/* 'For' executado apenas para posicionar "aux" */
				for (aux = getAdj(G)[vInicial], aux2 = next(aux); !isEqual(getArc(aux2), arcAux); aux = aux2, aux2 = next(aux));
				arcAux2 = getArc(removeNext(aux));
				insertArc(tree[vInicial], arcAux2);
				insertArc(tree[i], arcAux2);
			}
			/* Caso não haja arco "i->vInicial" nem "vInicial->i", criamos S.P.G., um arco artificial "vInicial->i" de custo infinito 
			   (a menos que i seja a origem, aí criamos um arco de i->vInicial)                                                      */
			else{
				if(i != origem) {
					arcAux2 = newArc(vInicial, i, A, 0);
					insertArc(tree[vInicial], arcAux2);
					insertArc(tree[i], arcAux2);
				}
				else {
					arcAux2 = newArc(i, vInicial, A, 0);
					insertArc(tree[i], arcAux2);
					insertArc(tree[vInicial], arcAux2);
				}
			} 

		}
	}

	/* Procuramos por um arco com vertice inicial em "origem" entre os adjacentes a "origem"
	   e com vértice final em "vInicial", e então inserimos todo o fluxo nesse arco, dado 
	   que só há 1 fonte no nosso problema em questão.                                       */
	setFlow(hasArc(tree[origem], origem, vInicial), qtdProduto);

	/* Procuramos por um arco com vértice final em "destino" entre os adjacentes a "destino"
	   e com vértice inicial em "vInicial", e então inserimos todo o fluxo nesse arco, dado
	   que só há 1 sorvedouro no nosso problema em questão.                                 */
	setFlow(hasArc(tree[destino], vInicial, destino), qtdProduto);


	/* Vamos, agora, encontrar uma primeira solução factível (árvore válida) para iniciarmos o simplex. */
	solucaoInicial = resolveSimplex(tree, custosArtificiais, origem, destino, G, 2, qtdProduto);

	for(i = 0; i < V; i++){
		/* Como a lista tem cabeça, "next(solucaoInicial[i]" é o 1o elemento dela */
		for(aux = next(solucaoInicial[i]); aux != NULL; aux = aux->next){
			if(custosArtificiais[getCost(getArc(aux))] == 1 && getFlow(getArc(aux)) != 0){
				printf("O problema é inviável (A solução inicial exige a presença de um arco artificial. (O arco %d -> %d)\n", 
					    getVertexX(getArc(aux)), getVertexW(getArc(aux)));
				return 0;
			} 
		}
	}


	/********************************************************************************************************************/
	/* A partir daqui, entramos na 2ª fase do programa, quando executamos o algoritmo para resolver o problema de fato. */
	/********************************************************************************************************************/
	printf("Executando Simplex... \n");

	solucaoFinal = resolveSimplex(solucaoInicial, custosReais, origem, destino, G, infinito+1, qtdProduto);
	/* Nossa solucaoFinal sera uma arvore que minimiza a função objetivo */

	/********************************************************************************************************/
	/* A partir daqui, entramos na 3ª fase do programa, onde imprimimos a solução final no terminal.        */
	/********************************************************************************************************/


	printf("Solução Final : \n");

	custoFinal = 0;
	printf("Arcos na árvore de custos mínimos: ");
	for(i = 0; i < V; i++){
		/* Varremos a árvore da solução final, eliminando arco a arco e imprimindo seus dados na resposta */
		for(aux = solucaoFinal[i]; next(aux) != NULL; ){
			arcAux = getArc(next(aux));
			auxX = getVertexX(arcAux);
			auxY = getVertexW(arcAux);
			printf("%d -> %d custo : %d  fluxo : %d \n", 
			        getVertexX(arcAux), getVertexW(arcAux), custosReais[getCost(arcAux)], getFlow(arcAux));
			custoFinal += (custosReais[getCost(arcAux)]) * getFlow(arcAux);
			freeList(removeFromTree(solucaoFinal, auxX, auxY));
		}
	}

	
	printf("\n Arcos fora da árvore : \n");

	for(i = 0; i < V; i++){
		for(aux = getAdj(G)[i]; next(aux) != NULL; ){
			arcAux = getArc(next(aux));
			printf("%d -> %d custo : %d  fluxo : 0 \n", 
			        getVertexX(arcAux), getVertexW(arcAux), custosReais[getCost(arcAux)]);
			freeList(removeNext(aux));
		}
	}


	printf("Custo final (valor da função objetivo) :  %d \n", custoFinal);


	return 0;
}