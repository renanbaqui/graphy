#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "parse.h"
#include "fila.h"
#include <limits.h>

int DEBUG_messages = 1;

int **AdicionaVertice(int **, int );
int **RemoveVertice(int **, int *, int);
void RemoveAresta(int **, int, int);
void AdicionaAresta(int **, int, int);
void Vizinhos(int **, int, int);
void ImprimeMatriz(int **, int);
void Busca(int **, int, int, int **, int **, int *);
void BuscaGrafo(int **, int, int **, int **, int *);
void BuscaCompleta(int **, int, int **, int **, int *);
int EhConexo(int **, int, int **, int **, int *);
int TemCiclo(int **, int, int **, int **, int *);
int EhFloresta(int **, int, int **, int **, int *);
int EhArvore(int **, int, int **, int **, int *);
void ObterFlorestaGeradora(int **, int, int **, int **, int *);
void BuscaLargura(int **, int, int, int **, int **, int *, int);
void DeterminarDistancias(int **, int, int, int **, int **, int *, int, int *);
void BuscaProfundidade(int **, int, int, int **, int **, int *, int);
void BuscaProfundidadeVisita(int **, int, int, int **, int **, int *, int);
void prepare_dump_json(int *ve,int **ar,int *vertices, int *arestas,char *fileName, int debug, int **);
void ImprimeVizinhosDeVertice(int **, int, int);
int prepare_fordfulk(int *,int **,int *, int *, int *, int , int , int );
int fordFulkerson(int **, int , int , int, int);

int main(int argc,char* argv[]) {
	int LIMITE_ARESTAS = 400000;
	int v[1000];
	int c[10000]; 	//Edge capacity limit in directed graphs. Edge limit for directed graphs is 10,000.
  	int json_ = 0;

	int **a = NULL;
		a = (int **)malloc(LIMITE_ARESTAS * sizeof(int *));
		int i_;
		for (i_ = 0; i_ < LIMITE_ARESTAS; i_++) {
		a[i_] = (int *)malloc(2 * sizeof(int));
    }

	
	FILE *Arquivo = NULL;
	int **Matriz = NULL;
	int *Visitado;
	int **Descoberta;
	int **Explorada;
	int vertices = 0;
	int arestas = 0;
	int i, j, v1, v2;
	int *Dist;

	char BuscaGrafo_[] = "BuscaGrafo"; 
	char BuscaCompleta_[] = "BuscaCompleta";
	char EhConexo_[] = "EhConexo";
	char EhArvore_[] = "EhArvore";   
	char TemCiclo_[] = "TemCiclo";
	char EhFloresta_[] = "EhFloresta";  
	char BuscaLargura_[] = "BuscaLargura";  
	char BuscaProfundidade_[] = "BuscaProfundidade";  
	char DeterminarDistancias_[] = "DeterminarDistancias"; 
	char ImprimeGrafo_[] = "ImprimeMatriz"; 
	char ExcluiAresta_[] = "RemoveAresta"; 
	char ExcluiVertice_[] = "RemoveVertice"; 
	char AdicionaAresta_[] = "AdicionaAresta"; 
	char AdicionaVertice_[] = "AdicionaVertice"; 
	char ObterFloresta_[] = "ObterFlorestaGeradora"; 
	char Vizinhos_[] = "Vizinhos";
	char FordFulk_[] = "FordFulk";

	if(argc<3){ 
		printf("Empty paremeters: Type this: matriz-en NAME_FILE METHOD");
		return 0;
	}
	if(argc>=4){ 
		// O terceiro parâmetro permite enviar como saida somente o tempo de execução.
		DEBUG_messages = atoi(argv[3]);
	}


    char json_ext[] = ".json"; 
    // Se o parametro tem extensão .json, então processa com funções de parse.c (incluido com header).
    if(strstr(argv[1],json_ext)==NULL){
      	if(DEBUG_messages==1) printf("Arquivo txt\n");
    }else{
		if(DEBUG_messages==1) printf("Arquivo json\n");
		json_ = 1;
		if(strcmp(argv[2],FordFulk_)==0){
			parse_json_capacity(v,a,c,&vertices,&arestas,argv[1],DEBUG_messages);
		}else{
			parse_json(v,a,&vertices,&arestas,argv[1],DEBUG_messages);
		}
      
    }

	if(json_==0){
		
		Arquivo = fopen(argv[1], "r");
		fscanf(Arquivo, "%d\t%d", &vertices, &arestas);
		
	}

	Matriz = (int **)malloc(vertices * sizeof(int *));
	for (i = 0; i < vertices; i++) {
		Matriz[i] = (int *)malloc(vertices * sizeof(int));
	}

	for (i = 0; i < vertices; i++) {
		for (j = 0; j < vertices; j++) {
			Matriz[i][j] = 0;
		}
	}
	

	
	for (i = 0; i < arestas; i++) {
		 if(json_){
        v1 = a[i][0];
        v2 = a[i][1];
      }else{
				fscanf(Arquivo, "%d\t%d", &v1, &v2);	
			}
		
		Matriz[v1][v2] = 1;
		Matriz[v2][v1] = 1;
	}

	
  int arvore, conexo, ciclo, floresta;

  clock_t tempo1; 
	tempo1 = clock();



    if(DEBUG_messages==1) printf("Executando: %s\n",argv[2]);
		if(strcmp(argv[2],BuscaGrafo_)==0){
			BuscaGrafo(Matriz,vertices, Descoberta,Explorada,Visitado);	
		}
		else if (strcmp(argv[2],ImprimeGrafo_)==0){			
			ImprimeMatriz(Matriz, vertices);
		}
    else if (strcmp(argv[2],ExcluiAresta_)==0){
			if(argc>=7){
				RemoveAresta(Matriz,atoi(argv[5]),atoi(argv[6]));
			}
			else{
				RemoveAresta(Matriz,3,4);
			}
		
			if(argc>=5) prepare_dump_json(v,a,&vertices,&arestas,argv[4],DEBUG_messages,Matriz);
		}
    else if (strcmp(argv[2],ExcluiVertice_)==0){
			int **NovaMatriz;
			if(argc>=6){
				NovaMatriz = RemoveVertice(Matriz,&vertices,atoi(argv[5]));				
				
			}else{
				NovaMatriz = RemoveVertice(Matriz,&vertices,4);	
			}
			
			vertices--;
			if(argc>=5) prepare_dump_json(v,a,&vertices,&arestas,argv[4],DEBUG_messages,NovaMatriz);
		}
    else if (strcmp(argv[2],AdicionaAresta_)==0){
		 if(argc>=7){
			AdicionaAresta(Matriz,atoi(argv[5]),atoi(argv[6]));
		 }else{  
			
			 switch(vertices){
				case 5:
				case 10:
					AdicionaAresta(Matriz,2,4);
					break;   
				case 6:
				case 7:
				case 9:
					AdicionaAresta(Matriz,1,4);
					break;      
				case 8:
					AdicionaAresta(Matriz,1,7);
					break;
				case 20:
					AdicionaAresta(Matriz,7,10);
					break;
				case 50:
					AdicionaAresta(Matriz,25,1);
					break;
				case 100:
					AdicionaAresta(Matriz,79,57);
					break;
				case 200:
					AdicionaAresta(Matriz,81,69);
					break;
				default:
					AdicionaAresta(Matriz,31,0);
				}
			}

			if(argc>=5) prepare_dump_json(v,a,&vertices,&arestas,argv[4],DEBUG_messages,Matriz);
			
		}
		else if (strcmp(argv[2],BuscaCompleta_)==0){
			BuscaCompleta(Matriz,vertices, Descoberta,Explorada,Visitado);
		}
		else if (strcmp(argv[2],EhConexo_)==0){
			conexo = EhConexo(Matriz,vertices,Descoberta,Explorada,Visitado);
			if(DEBUG_messages==3) printf("Conexo?: %d\n",conexo);
		}
		else if (strcmp(argv[2],EhArvore_)==0){
			arvore = EhArvore(Matriz,vertices,Descoberta,Explorada,Visitado);
			if(DEBUG_messages==3) printf("Arvore?: %d\n",arvore);
		}
		else if (strcmp(argv[2],TemCiclo_)==0){
			ciclo = TemCiclo(Matriz,vertices,Descoberta,Explorada,Visitado);
			if(DEBUG_messages==3) printf("Ciclo?: %d\n",ciclo);
		}
		else if (strcmp(argv[2],EhFloresta_)==0){
			floresta = EhFloresta(Matriz, vertices, Descoberta, Explorada, Visitado);
			if(DEBUG_messages==3) printf("Floresta?: %d\n",floresta);
		}
		else if(strcmp(argv[2],BuscaLargura_)==0){
			BuscaLargura(Matriz,vertices,arestas,Descoberta,Explorada,Visitado,0);
		}
		else if(strcmp(argv[2],DeterminarDistancias_)==0){
			DeterminarDistancias(Matriz,vertices,arestas,Descoberta,Explorada,Visitado,0,Dist);
		}
		else if(strcmp(argv[2],BuscaProfundidade_)==0){
			BuscaProfundidade(Matriz,vertices,arestas,Descoberta,Explorada,Visitado,0);
		}
		else if(strcmp(argv[2],ObterFloresta_)==0){
			ObterFlorestaGeradora(Matriz, vertices, Descoberta, Explorada, Visitado);
		}
		else if(strcmp(argv[2],AdicionaVertice_)==0){
			int **NovaMatriz = AdicionaVertice(Matriz, vertices);
			int newVertices = vertices;
			newVertices = newVertices + 1;
			if(argc>=5) prepare_dump_json(v,a,&newVertices,&arestas,argv[4],DEBUG_messages,NovaMatriz);
		} 
		else if(strcmp(argv[2],Vizinhos_)==0){
      		if(argc>=5) {          
        		ImprimeVizinhosDeVertice(Matriz,vertices,atoi(argv[4]));
      		}
		}
		if(strcmp(argv[2],FordFulk_)==0){
			int result = prepare_fordfulk(v, a, c, &vertices, &arestas, DEBUG_messages, atoi(argv[4]), atoi(argv[5]));
			if(DEBUG_messages==3) printf("O maximo fluxo possível eh: %d\n",result);
		}
		
float toMiliseconds = 1000/CLOCKS_PER_SEC;
float interval = (clock() - tempo1) * toMiliseconds;
  
if(DEBUG_messages==1) printf("Tempo de %s: %.3f\n",argv[2],interval);
if(DEBUG_messages==0) printf("%.3f\n",interval);
if(DEBUG_messages==4) printf("%d,%.3f\n",vertices,interval);

 
	
	for (i = 0; i < vertices; i++) {
		free(Matriz[i]);
	}

	free(Matriz);
	free(a);

	fclose(Arquivo);
		
	return 0;
}

void BuscaGrafo(
	int **Matriz,
	int vertices,
	int **Descoberta,
	int **Explorada,
	int *Visitado) {
	
	// Cria lista de visitados.

	Visitado = (int *)malloc(vertices * sizeof(int));
	int i, j;
	for (i = 0; i < vertices; i++) {
		Visitado[i] = 0;
	}

	// Cria lista aresta explorada.

	Explorada = (int **)malloc(vertices * sizeof(int *));
	for (i = 0; i < vertices; i++) {
		Explorada[i] = (int *)malloc(vertices * sizeof(int));
	}

	for (i = 0; i < vertices; i++) {
		for (j = 0; j < vertices; j++) {
			Explorada[i][j] = 0;
		}
	}

	// Cria lista aresta descoberta.

	Descoberta = (int **)malloc(vertices * sizeof(int *));
	for (i = 0; i < vertices; i++) {
		Descoberta[i] = (int *)malloc(vertices * sizeof(int));
	}

	for (i = 0; i < vertices; i++) {
		for (j = 0; j < vertices; j++) {
			Descoberta[i][j] = 0;
		}
	}

	Busca(Matriz, vertices, 0, Descoberta, Explorada, Visitado);

  


}

void Busca(
	int **Matriz,
	int vertices,
	int vertice,
	int **Descoberta,
	int **Explorada,
	int *Visitado) {
	 

	if (Matriz[vertice]) {
		Visitado[vertice] = 1;
		if(DEBUG_messages==3) printf("Visito o vertice %d\n",vertice);
	}

	int i, j;
	for (i = 0; i < vertices; i++) {
		for (j = 0; j < vertices; j++) {
			// Verifica se a aresta existe, se o vertice ja foi visitado e se a aresta nao foi explorada.
			if (i < j && Matriz[i][j] == 1){
				if(DEBUG_messages==3) printf("Vamos da aresta %d a %d\n",i,j);
				if(Visitado[i] == 1 && Explorada[i][j] == 0) {
					Explorada[i][j] = 1;				
					Explorada[j][i] = 1;				
					if (Visitado[j] == 0) { 	// Se o vertice j nao for visitado, ele
									// visita e descobre a aresta.
						Visitado[j] = 1;
						if(DEBUG_messages==3) printf("Visito o vertice %d\n",j);
						if(DEBUG_messages==3) printf("Descoberta a aresta %d-%d\n",i,j);
						Descoberta[i][j] = 1;
						Descoberta[j][i] = 1;
			
					}
				}
			}
		}
	}
	
}

void BuscaCompleta(
	int **Matriz,
	int vertices,
	int **Descoberta,
	int **Explorada,
	int *Visitado) {
	
	// Cria lista de visitados.

	Visitado = (int *)malloc(vertices * sizeof(int));
	int i, j;
	for (i = 0; i < vertices; i++) {
		Visitado[i] = 0;
	}

	// Cria lista aresta explorada.

	Explorada = (int **)malloc(vertices * sizeof(int *));
	for (i = 0; i < vertices; i++) {
		Explorada[i] = (int *)malloc(vertices * sizeof(int));
	}

	for (i = 0; i < vertices; i++) {
		for (j = 0; j < vertices; j++) {
			Explorada[i][j] = 0;
		}
	}

	// Cria lista aresta descoberta.

	Descoberta = (int **)malloc(vertices * sizeof(int *));
	for (i = 0; i < vertices; i++) {
		Descoberta[i] = (int *)malloc(vertices * sizeof(int));
	}

	for (i = 0; i < vertices; i++) {
		for (j = 0; j < vertices; j++) {
			Descoberta[i][j] = 0;
		}
	}

	for (i = 0; i < vertices; i++) {
		if (Visitado[i] == 0) {
			Busca(Matriz, vertices, i, Descoberta, Explorada, Visitado);
		}
	}

  	free(Descoberta);
	free(Explorada);
	free(Visitado);   
}

int EhConexo(
	int **Matriz,
	int vertices,
	int **Descoberta,
	int **Explorada,
	int *Visitado) {
	 
	 //Cria lista de visitados.

	Visitado = (int *)malloc(vertices * sizeof(int));
	int i, j;
	for (i = 0; i < vertices; i++) {
		Visitado[i] = 0;
	}

	// Cria lista aresta explorada.

	Explorada = (int **)malloc(vertices * sizeof(int *));
	for (i = 0; i < vertices; i++) {
		Explorada[i] = (int *)malloc(vertices * sizeof(int));
	}

	for (i = 0; i < vertices; i++) {
		for (j = 0; j < vertices; j++) {
			Explorada[i][j] = 0;
		}
	}

	// Cria lista aresta descoberta.

	Descoberta = (int **)malloc(vertices * sizeof(int *));
	for (i = 0; i < vertices; i++) {
		Descoberta[i] = (int *)malloc(vertices * sizeof(int));
	}

	for (i = 0; i < vertices; i++) {
		for (j = 0; j < vertices; j++) {
			Descoberta[i][j] = 0;
		}
	}

	Busca(Matriz, vertices, 0, Descoberta, Explorada, Visitado);

	for (i = 0; i < vertices; i++) {
		if (Visitado[i] == 0) {
			if(DEBUG_messages==3) printf("Vertice não visitado: %d\n",i);
			return 0;
		}
	}
  	free(Descoberta);
	free(Explorada);
	free(Visitado);

	 
   return 1;
}

int TemCiclo(
	int **Matriz,
	int vertices,
	int **Descoberta,
	int **Explorada,
	int *Visitado) {
	 

	// Cria lista de visitados.

	Visitado = (int *)malloc(vertices * sizeof(int));
	int i, j;
	for (i = 0; i < vertices; i++) {
		Visitado[i] = 0;
	}

	// Cria lista aresta explorada.

	Explorada = (int **)malloc(vertices * sizeof(int *));
	for (i = 0; i < vertices; i++) {
		Explorada[i] = (int *)malloc(vertices * sizeof(int));
	}

	for (i = 0; i < vertices; i++) {
		for (j = 0; j < vertices; j++) {
			Explorada[i][j] = 0;
		}
	}

	// Cria lista aresta descoberta.

	Descoberta = (int **)malloc(vertices * sizeof(int *));
	for (i = 0; i < vertices; i++) {
		Descoberta[i] = (int *)malloc(vertices * sizeof(int));
	}

	for (i = 0; i < vertices; i++) {
		for (j = 0; j < vertices; j++) {
			Descoberta[i][j] = 0;
		}
	}

	for (i = 0; i < vertices; i++) {
		if (Visitado[i] == 0) {
			Busca(Matriz, vertices, i, Descoberta, Explorada, Visitado);
		}
	}

	for (i = 0; i < vertices; i++) {
		for (j = 0; j < vertices; j++) {
			if (i < j && Matriz[i][j] && Descoberta[i][j] == 0) {
				if(DEBUG_messages==3) printf("Aresta nao descoberta %d-%d\n",i,j);
				
        		return 1;
			}
		}
	}


	free(Descoberta);
	free(Explorada);
	free(Visitado);
 	if(DEBUG_messages==3) printf("Todas arestas descobertas.\n");
	 
   	return 0;
}

int EhFloresta(
	int **Matriz,
	int vertices,
	int **Descoberta,
	int **Explorada,
	int *Visitado) {
	 
	if (TemCiclo(Matriz, vertices, Descoberta, Explorada, Visitado) == 0) {
		
    return 1;
	}
  
  
  return 0;
	free(Descoberta);
	free(Explorada);
	free(Visitado);
	
   
}

int EhArvore(
	int **Matriz,
	int vertices,
	int **Descoberta,
	int **Explorada,
	int *Visitado) {
	 clock_t tempo; //teste de tempo iniciado
	 tempo = clock();

	// Cria lista de visitados.

	Visitado = (int *)malloc(vertices * sizeof(int));
	int i, j;
	for (i = 0; i < vertices; i++) {
		Visitado[i] = 0;
	}

	// Cria lista aresta explorada.

	Explorada = (int **)malloc(vertices * sizeof(int *));
	for (i = 0; i < vertices; i++) {
		Explorada[i] = (int *)malloc(vertices * sizeof(int));
	}

	for (i = 0; i < vertices; i++) {
		for (j = 0; j < vertices; j++) {
			Explorada[i][j] = 0;
		}
	}

	// Cria lista aresta descoberta.

	Descoberta = (int **)malloc(vertices * sizeof(int *));
	for (i = 0; i < vertices; i++) {
		Descoberta[i] = (int *)malloc(vertices * sizeof(int));
	}

	for (i = 0; i < vertices; i++) {
		for (j = 0; j < vertices; j++) {
			Descoberta[i][j] = 0;
		}
	}

	Busca(Matriz, vertices, 0, Descoberta, Explorada, Visitado);

	for (i = 0; i < vertices; i++) {

		if (Visitado[i] == 0) {
			
      return 0;
		}
	}

	for (i = 0; i < vertices; i++) {
		if (Visitado[i] == 0) {
			Busca(Matriz, vertices, i, Descoberta, Explorada, Visitado);
		}
	}

	for (i = 0; i < vertices; i++) {
		for (j = 0; j < vertices; j++) {
			if (Descoberta[i][j] == 0) {
				
        return 0;
			}
		}
	}
	
  return 1;
	free(Descoberta);
	free(Explorada);
	free(Visitado);

	
}

void ObterFlorestaGeradora(
	int **Matriz,
	int vertices,
	int **Descoberta,
	int **Explorada,
	int *Visitado) {
	


	int i, j;
	int **Grafo;
	Grafo = (int **)malloc(vertices * sizeof(int *));
	for (i = 0; i < vertices; i++) {
		Grafo[i] = (int *)malloc(vertices * sizeof(int));
	}
	
	// Cria lista de visitados.

	Visitado = (int *)malloc(vertices * sizeof(int));

	for (i = 0; i < vertices; i++) {
		Visitado[i] = 0;
	}

	// Cria lista aresta explorada.

	Explorada = (int **)malloc(vertices * sizeof(int *));
	for (i = 0; i < vertices; i++) {
		Explorada[i] = (int *)malloc(vertices * sizeof(int));
	}

	for (i = 0; i < vertices; i++) {
		for (j = 0; j < vertices; j++) {
			Explorada[i][j] = 0;
		}
	}

	// Cria lista aresta descoberta.

	Descoberta = (int **)malloc(vertices * sizeof(int *));
	for (i = 0; i < vertices; i++) {
		Descoberta[i] = (int *)malloc(vertices * sizeof(int));
	}

	for (i = 0; i < vertices; i++) {
		for (j = 0; j < vertices; j++) {
			Descoberta[i][j] = 0;
		}
	}

	for (i = 0; i < vertices; i++) {
		if (Visitado[i] == 0) {
			Busca(Matriz, vertices, i, Descoberta, Explorada, Visitado);
		}
	}

	for (i = 0; i < vertices; i++) {
		for (j = 0; j < vertices; j++) {
			if (Descoberta[i][j] == 1) {
				Grafo[i][j] = 1;
			} else {
				Grafo[i][j] = 0;
			}
		}
	}

   if(DEBUG_messages==3) printf("Para a arvore geradora desse grafo temos:\n");
	if(DEBUG_messages==3) ImprimeMatriz(Grafo, vertices);


	free(Grafo);
  for(i=0;i<vertices;i++){
    free(Descoberta[i]);
	  free(Explorada[i]);
  }
	free(Visitado);

	
}

void ImprimeMatriz(int **Matriz, int n) {
	int i, j;

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			if (Matriz[i][j] && i < j) {
				printf("Existe aresta entre %d e %d\n", i, j);
			}
		}
	}

	printf("\n\n");
}

void ImprimeVizinhosDeVertice(int **Matriz, int n,int v)
{
 int i, j;
	printf("Vizinhos do vertice %d\n",v);
	for (j = 0; j < n; j++) {
		if (Matriz[v][j] && v < j) {
			printf("Vertice %d\n", j);
		}
	}
	
}



void RemoveAresta(int **Matriz, int v1, int v2) {
	Matriz[v1][v2] = 0;
	Matriz[v2][v1] = 0;
}

void AdicionaAresta(int **Matriz, int v1, int v2) {
	Matriz[v1][v2] = 1;
	Matriz[v2][v1] = 1;
}

void Vizinhos(int **Matriz, int n, int v) {
	int i, j;
	int TemVizinho = 0;

	printf("Vizinhos do vertice %d:\n", v);

	for (i = 0; i < n; i++) {
		if (Matriz[v][i]) {
			printf("Vertice %d\n", i);
			TemVizinho = 1;
		}
	}

	if (TemVizinho == 0) {
		printf("Nenhum!\n");
	}
}

int **AdicionaVertice(int **Matriz, int N) {
	int **MatrizNova = NULL;
	int i, j;
	int Vertices = (N);
	int NovoVertices = (N) + 1;


	MatrizNova = (int **)malloc(NovoVertices * sizeof(int *));
	for (i = 0; i < NovoVertices; i++) {
		MatrizNova[i] = (int *)malloc(NovoVertices * sizeof(int));
	}

	for (i = 0; i < Vertices; i++) {
		for (j = 0; j < Vertices; j++) {
			MatrizNova[i][j] = Matriz[i][j];
		}
	}

	for (i = 0; i < NovoVertices; i++) {
		MatrizNova[i][Vertices] = 0;
		MatrizNova[Vertices][i] = 0;
	}

	if(DEBUG_messages==3) printf("Adicionado Vertice\n");

	return MatrizNova;
}

int **RemoveVertice(int **Matriz, int *N, int V) {
	int **MatrizNova = NULL;
	int i, j;
	int Vertices = (*N);
	int NovoVertices = (*N) - 1;

 	if(DEBUG_messages==3) printf("Criando matriz nova com numero de vertices = %d\n", NovoVertices);
	MatrizNova = (int **)malloc(NovoVertices * sizeof(int *));
	for (i = 0; i < NovoVertices; i++) {
		MatrizNova[i] = (int *)malloc(NovoVertices * sizeof(int));
	}



	for (i = 0; i < Vertices; i++) {
		for (j = 0; j < Vertices; j++) {
			if (i < V && j < V) {
				MatrizNova[i][j] = Matriz[i][j];
			} else if (i < V && j > V) {
				MatrizNova[i][j - 1] = Matriz[i][j];
			} else if (i > V && j < V) {
				MatrizNova[i - 1][j] = Matriz[i][j];
			} else if (i > V && j > V) {
				MatrizNova[i - 1][j - 1] = Matriz[i][j];
			}
		}
	}



	return MatrizNova;
}



void BuscaLargura(
	int **Matriz,
	int vertices,
	int arestas,
	int **Descoberta,
	int **Explorada,
	int *Visitado,
	int v) {


	Visitado = (int *)malloc(vertices * sizeof(int));
	int i, j;
	for (i = 0; i < vertices; i++) {
		Visitado[i] = 0;
	}

	// Cria lista aresta explorada.

	Explorada = (int **)malloc(arestas * sizeof(int *));
	for (i = 0; i < arestas; i++) {
		Explorada[i] = (int *)malloc(arestas * sizeof(int));
	}

	for (i = 0; i < arestas; i++) {
		for (j = 0; j < arestas; j++) {
			Explorada[i][j] = 0;
		}
	}

	// Cria lista aresta descoberta.

	Descoberta = (int **)malloc(arestas * sizeof(int *));
	for (i = 0; i < arestas; i++) {
		Descoberta[i] = (int *)malloc(arestas * sizeof(int));
	}

	for (i = 0; i < arestas; i++) {
		for (j = 0; j < arestas; j++) {
			Descoberta[i][j] = 0;
		}
	}

	resetFila();
	Visitado[v]=1;
	entraFila(v);
	while(!filaVazia()){
			int u = sacaFila();
			if(DEBUG_messages==3) printf("Removo o vertice:%d\n",u);
      if(DEBUG_messages==3) printFila();
			int w;
        for(w=0;w<vertices;w++){ 
            if(Matriz[u][w]==1){ //Para todos os vizinhos desse vertice
                if (Visitado[w]==1){
                    if(Explorada[u][w]==0){
											Explorada[u][w]=1;
											Explorada[w][u]=1;
										}
                }
								else {
									Explorada[u][w]=1;
									Explorada[w][u]=1;									
									Descoberta[u][w]=1;
									Descoberta[w][u]=1;
									Visitado[w]=1;
									if(DEBUG_messages==3) printf("Insiro:%d\n",w);
									entraFila(w);
								}
            }
        }
	}

  	free(Descoberta);
	free(Explorada);
	free(Visitado);

}


void DeterminarDistancias(
	int **Matriz,
	int vertices,
	int arestas,
	int **Descoberta,
	int **Explorada,
	int *Visitado,
	int v,
	int *Dist) {

	Dist = (int *)malloc(vertices * sizeof(int));
	int i, j;
	for (i = 0; i < vertices; i++) {
		Dist[i] = INT_MAX;
	}

	Visitado = (int *)malloc(vertices * sizeof(int));
	for (i = 0; i < vertices; i++) {
		Visitado[i] = 0;
	}

	// Cria lista aresta explorada.

	Explorada = (int **)malloc(vertices * sizeof(int *));
	for (i = 0; i < vertices; i++) {
		Explorada[i] = (int *)malloc(vertices * sizeof(int));
	}

	for (i = 0; i < vertices; i++) {
		for (j = 0; j < vertices; j++) {
			Explorada[i][j] = 0;
		}
	}

	// Cria lista aresta descoberta.

	Descoberta = (int **)malloc(vertices * sizeof(int *));
	for (i = 0; i < vertices; i++) {
		Descoberta[i] = (int *)malloc(vertices * sizeof(int));
	}

	for (i = 0; i < vertices; i++) {
		for (j = 0; j < vertices; j++) {
			Descoberta[i][j] = 0;
		}
	}

	resetFila();
	Visitado[v]=1;
	entraFila2(v,1);
	Dist[v]=0;
	while(!filaVazia()){
			struct par casal = sacaFila2();
			int u = casal.value1;
			int nivel = casal.value2;
			if(DEBUG_messages==3) printf("Removo o vertice:%d, nivel=%d\n",u,nivel);
      if(DEBUG_messages==3) printFila();
			int w;
        for(w=0;w<vertices;w++){ 
            if(Matriz[u][w]==1){ //Para todos os vizinhos desse vertice.
                if (Visitado[w]==1){
                    if(Explorada[u][w]==0){
											Explorada[u][w]=1;
											Explorada[w][u]=1;
										}
                }
								else {
									Explorada[u][w]=1;
									Explorada[w][u]=1;									
									Descoberta[u][w]=1;
									Descoberta[w][u]=1;
									Visitado[w]=1;
									Dist[w]=nivel;
									if(DEBUG_messages==3) printf("Insiro:%d\n",w);
									entraFila2(w,nivel+1);
								}
            }
        }
	}

	
	
  	free(Descoberta);
	free(Explorada);
	free(Visitado);
	free(Dist);

}

void BuscaProfundidade(
	int **Matriz,
	int vertices,
	int arestas,
	int **Descoberta,
	int **Explorada,
	int *Visitado,
	int v) {


	Visitado = (int *)malloc(vertices * sizeof(int));
	int i, j;
	for (i = 0; i < vertices; i++) {
		Visitado[i] = 0;
	}

	// Cria lista aresta explorada.

	Explorada = (int **)malloc(vertices * sizeof(int *));
	for (i = 0; i < vertices; i++) {
		Explorada[i] = (int *)malloc(vertices * sizeof(int));
	}

	for (i = 0; i < vertices; i++) {
		for (j = 0; j < vertices; j++) {
			Explorada[i][j] = 0;
		}
	}

	// Cria lista aresta descoberta.

	Descoberta = (int **)malloc(vertices * sizeof(int *));
	for (i = 0; i < vertices; i++) {
		Descoberta[i] = (int *)malloc(vertices * sizeof(int));
	}

	for (i = 0; i < vertices; i++) {
		for (j = 0; j < vertices; j++) {
			Descoberta[i][j] = 0;
		}
	}

	BuscaProfundidadeVisita(Matriz,vertices,arestas,Descoberta,Explorada,Visitado,v);


  	free(Descoberta);
	free(Explorada);
	free(Visitado);

}


void BuscaProfundidadeVisita(
	int **Matriz,
	int vertices,
	int arestas,
	int **Descoberta,
	int **Explorada,
	int *Visitado,
	int u){

	if(DEBUG_messages==3) printf("Visita:%d\n",u);
	Visitado[u]=1;

		int w;
		for(w=0;w<vertices;w++){ 
				if(Matriz[u][w]==1){ // Para todos os vizinhos desse vertice.
						if (Visitado[w]==1){
								if(Explorada[u][w]==0){
									Explorada[u][w]=1;
									Explorada[w][u]=1;
								}
						}
						else {
							Explorada[u][w]=1;
							Explorada[w][u]=1;									
							Descoberta[u][w]=1;
							Descoberta[w][u]=1;
							BuscaProfundidadeVisita(Matriz,vertices,arestas,Descoberta,Explorada,Visitado,w);
							
						}
				}
		}
}


void prepare_dump_json(int *ve,int **ar,int *vertices, int *arestas,char *fileName, int debug, int **Matriz){
	int i, j;
	int aresta = 0;
	for (i = 0; i < *vertices; i++) {
		for (j = 0; j < *vertices; j++) {
			if (Matriz[i][j] && i < j) {
				ar[aresta][0]=i;
				ar[aresta][1]=j;
				aresta++;
			}
		}
	}
	*arestas = aresta;
	if(debug==3) printf("Revisados: %d vertices e %d arestas.\n",*vertices,aresta);
	dump_json(ve,ar,vertices,arestas,fileName,debug);	
}

// Retorna 1 se existe um caminho desde fonte s até fonte t no grafo residual 
// e também preenche o pai (parent[]) para guardar o caminho   
 
int BuscaLarguraFord(int **rGraph, int s, int t, int parent[], int vertices) 
{ 
	if(DEBUG_messages==3) printf("Busca?: s=%d t=%d\n",s,t);
   
    int visited[vertices]; 
    memset(visited, 0, sizeof(visited)); 
  
	resetFila();
	entraFila(s);
	
    visited[s] = 1; 
    parent[s] = -1; 
  
    while (!filaVazia()) 
    { 
		int u = sacaFila();
		int v;
        for (v=0; v<vertices; v++) 
        { 
            if (visited[v]==0 && rGraph[u][v] > 0) 
            { 
                entraFila(v);
                parent[v] = u; 
                visited[v] = 1; 
            } 
        } 
    } 
  
    // Se o fim (o vertice t) foi visitado começando desde a fonte (s), então retorna 1, senão 0.
    return (visited[t] == 1); 
} 
  
int min(int a, int b){
	if (a>b) return b;
	else return a;
}
// Retorna o fluxo máximo de s a t para o grafo. 
int fordFulkerson(int **grafo, int s, int t, int debug, int vertices) 
{ 
    int u, v, i, j; 
  
	int **rGraph = NULL;
	rGraph = (int **)malloc(vertices * sizeof(int *));
	for (i = 0; i < vertices; i++) {
		rGraph[i] = (int *)malloc(vertices * sizeof(int));
	}

	for (i = 0; i < vertices; i++) {
		for (j = 0; j < vertices; j++) {
			rGraph[i][j] = 0;	
		}
	}
   
    for (u = 0; u < vertices; u++) 
        for (v = 0; v < vertices; v++) 
             rGraph[u][v] = grafo[u][v]; 
  
    int parent[vertices];  // Array que sera preenchido por BuscaLargura Ford para guardar o caminho.
  
    int max_flow = 0;  // Não tem fluxo inicialmente. 
	 int count = 0;
    // Aumenta o fluxo enquanto existe um caminho da fonte (s) até o fim (t).  
    while (BuscaLarguraFord(rGraph, s, t, parent, vertices)) 
    { 
        // Achar a capacidade minima residual das arestas ao longo do caminho preenchido por BuscaLarguraFord.
           
        int path_flow = INT_MAX; 
        for (v=t; v!=s; v=parent[v]) 
        { 
            u = parent[v]; 
            if(DEBUG_messages==3) printf("min: %d %d u=%d v=%d\n",path_flow,rGraph[u][v],u,v);
			path_flow = min(path_flow, rGraph[u][v]); 			
        } 
		if(DEBUG_messages==3) printf("path_flow: %d \n",path_flow);
  
        // Atualizamos a capacidades residuais das arestas e arestas reversas ao longo do caminho.
        for (v=t; v != s; v=parent[v]) 
        { 
            u = parent[v]; 
			if(DEBUG_messages==3) printf("Capacidade de %d-%d: %d - %d \n",u,v,rGraph[u][v],path_flow);
            rGraph[u][v] -= path_flow; 
			if(DEBUG_messages==3) printf("Capacidade de %d-%d: %d + %d \n",v,u,rGraph[v][u],path_flow);
            rGraph[v][u] += path_flow; 
			
            
        } 
  
        // Acrescentamos o caminho do fluxo ao fluxo total.
        max_flow += path_flow; 
		count++;
        if(count>20000){
          printf("Superado numero maximo de iterações.\n");  
          break;
        }
    } 
  
    // Retornamos o fluxo total.
    return max_flow; 
} 


int prepare_fordfulk(int *ve,int **ar,int *ca, int *vertices, int *arestas, int debug, int s, int t){
	// Criamos uma matriz nova que não é simétrica, pois o grafo para fordfulk é direcionado.
	int **Matriz = NULL;
	int i,j, v1, v2;
	
	Matriz = (int **)malloc(*vertices * sizeof(int *));
	for (i = 0; i < *vertices; i++) {
		Matriz[i] = (int *)malloc(*vertices * sizeof(int));
	}

	for (i = 0; i < *vertices; i++) {
		for (j = 0; j < *vertices; j++) {
			Matriz[i][j] = 0;	
		}
	}

	// Le arestas e marca elas como existentes (iguais a capacidade) na matriz.
	for (i = 0; i < *arestas; i++) {	
        v1 = ar[i][0];
        v2 = ar[i][1];		
		Matriz[v1][v2] = ca[i];
		//Cada aresta é direcionada (i,j) não equivale a (j,i)
	}	
	
	return fordFulkerson(Matriz, s, t, debug, *vertices); 
	
}
