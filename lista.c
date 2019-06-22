#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "parse.h"
#include "fila.h"
#include <limits.h>

/* lista de adjacencia */

struct vizinhos
{
    int valor;
    int capacidad;
    struct vizinhos* prox;
};


struct vertice
{
    int valor;
    struct vertice* prox;
    struct vizinhos* listavizinhos;
};

struct ArestaRotulada
{
  int explorada;
  int descoberta;
  int v1;
  int v2;
};

struct vizinhos* criaVizinho(int);
struct vertice* criaVertice(int);
struct vertice* BuscaVertice(struct vertice*, int);
struct vizinhos* BuscaVizinho(struct vizinhos*, int);
void LimpaListaVertice(struct vertice **);
void LimpaListaVizinhos(struct vizinhos **);
void InsereVerticeFim(struct vertice**, int, int*);
void InsereVizinhoFim(struct vizinhos**, int);
void ExcluiVizinho(struct vizinhos**, int);
void ExcluiVertice(struct vertice**, int, int*);
void ExcluiAresta(struct vertice**, int, int);
void AdicionaAresta(struct vertice**, int, int);
void ImprimeVizinhos(struct vizinhos*);
void ImprimeGrafo(struct vertice*);

void BuscaGrafo(int, int**, struct ArestaRotulada**, int);
void BuscaCompleta(int**, struct ArestaRotulada**, int, int);
int EhConexo(int, int**, struct ArestaRotulada**, int, int);
int TemCiclo(int**, struct ArestaRotulada**, int, int);
int EhFloresta(int**, struct ArestaRotulada**, int, int);
int EhArvore(int, int**, struct ArestaRotulada**, int, int);
void ObterFlorestaGeradora(int**, struct ArestaRotulada**, struct vertice**, int, int);
void BuscaLargura(int, int**, struct ArestaRotulada**, int, int,struct vertice*);
void DeterminarDistancias(int, int**, struct ArestaRotulada**, int, int,struct vertice*, int**);
void BuscaProfundidade(int, int**, struct ArestaRotulada**, int, int,struct vertice*);
void BuscaProfundidadeVisita(int, int** , struct ArestaRotulada** , int , int , struct vertice*);
void prepare_dump_json(int *ve,int **ar, struct ArestaRotulada** E, int n, int m, struct vertice* p,char *fileName, int debug);
void ImprimeVizinhosDeVertice(struct vertice*, int);
int prepare_fordfulk(int *ve,int **ar,int *ca, struct ArestaRotulada** E, int vertices, int arestas, int debug, int s, int t);

int DEBUG_messages = 1;
int aux = 0;//for debugging some messages

int main(int argc,char* argv[])
{
	  int LIMITE_ARESTAS = 400000;
    int v[1000];
    int c[10000]; //limite para capacidade de arestas em grafos direcionados. Para eles o limite de arestas é 10000
    int json_ = 0;

    int **a = NULL;
    a = (int **)malloc(LIMITE_ARESTAS * sizeof(int *));
    int i_;
    for (i_ = 0; i_ < LIMITE_ARESTAS; i_++) {
      a[i_] = (int *)malloc(2 * sizeof(int));
    }

    
    //ListaVertice = no cabeca da lista encadeada
    //VerticesVisitados = vetor onde cada casa representa seu respectivo
    //vertice e seu valor (1 ou 0) representa se esse vertice foi visitado ou nao
    
    //ListaArestas = lista de arestas, seus vertices e se essas foram exploradas
    // e/ou descobertas (definida pelo struct ArestaRotulada acima da main)

    FILE* Arquivo = NULL; struct vertice* ListaVertice = NULL; 
    struct vertice* FlorestaGeradora = NULL;
    int VerticesLidos = 0; int arestas = 0; int i, j, v1, v2;
    int VerticesAdicionados = 0; int VerticesAdiFlor = 0; struct vertice* TempVert;
    int* VerticesVisitados = NULL; struct ArestaRotulada* ListaArestas = NULL;
    int* Dist = NULL;
    ////////////////////////////////////////////////////
	
    char BuscaGrafo_[] = "BuscaGrafo"; 
    char BuscaCompleta_[] = "BuscaCompleta";
    char EhConexo_[] = "EhConexo";
    char EhArvore_[] = "EhArvore";   
    char TemCiclo_[] = "TemCiclo";
    char EhFloresta_[] = "EhFloresta";  
    char BuscaLargura_[] = "BuscaLargura";  
    char BuscaProfundidade_[] = "BuscaProfundidade";  
    char ObterFlorestaGeradora_[] = "ObterFlorestaGeradora";  
    char DeterminarDistancias_[] = "DeterminarDistancias"; 
    char ImprimeGrafo_[] = "ImprimeGrafo"; 
    char ExcluiAresta_[] = "ExcluiAresta"; 
    char ExcluiVertice_[] = "ExcluiVertice"; 
    char AdicionaAresta_[] = "AdicionaAresta"; 
    char AdicionaVertice_[] = "AdicionaVertice";
    char Vizinhos_[] = "Vizinhos";
    char FordFulk_[] = "FordFulk";

    if(argc<3) 
    { 
      printf("Parametros vazios: Execute assim: matriz NOME_ARQUIVO METODO");
      return 0;
    }
    if(argc>=4) 
    { 
			//O terceiro parámetro permite enviar como saida somente o tempo de execução.
			DEBUG_messages = atoi(argv[3]);
    }

    char json_ext[] = ".json"; 
    //Se o parametro tem extensão .json, então procesa com funções de parse.c (incluido com header)
    if(strstr(argv[1],json_ext)==NULL){
      if(DEBUG_messages==1) printf("Arquivo txt\n");
    }else{
      if(DEBUG_messages==1) printf("Arquivo json\n");
      json_ = 1;
      if(strcmp(argv[2],FordFulk_)==0){
			  parse_json_capacity(v,a,c,&VerticesLidos,&arestas,argv[1],DEBUG_messages);
		  }else{
        parse_json(v,a,&VerticesLidos,&arestas,argv[1],DEBUG_messages);
      }
    }

    if(json_==0){
      //abre arquivo, le N e M
      Arquivo = fopen (argv[1],"r");
      fscanf(Arquivo, "%d\t%d", &VerticesLidos , &arestas);
      if(DEBUG_messages==1) printf("Lendo arquivo: %s\n",argv[1]);
      if(DEBUG_messages==1) printf("Vertices: %d - Arestas: %d\n",VerticesLidos,arestas);
      ////////////////////////////////////////////////////  
    }
    


    VerticesVisitados = (int*) malloc(sizeof(int) * VerticesLidos);
    Dist = (int*) malloc(sizeof(int) * VerticesLidos);
    ListaArestas = (struct ArestaRotulada*) malloc(sizeof(struct ArestaRotulada) * arestas);
    //ListaVertice = (struct vertice*) malloc(sizeof(struct vertice) * VerticesLidos);
   

    for(i = 0; i < VerticesLidos; i++)
    {
      InsereVerticeFim(&ListaVertice, i, &VerticesAdicionados);
      InsereVerticeFim(&FlorestaGeradora, i, &VerticesAdiFlor);
      VerticesVisitados[i] = 0;
      Dist[i]= INT_MAX;
    }


    for(i = 0; i < arestas; i++)
    {
      if(json_){
        v1 = a[i][0];
        v2 = a[i][1];
      }else{
        fscanf(Arquivo, "%d\t%d", &v1, &v2);        
      }
      //printf("Aresta %d: do vertice %d ao vertice %d\n",i,v1,v2);
      AdicionaAresta(&ListaVertice, v1, v2);
      ListaArestas[i].explorada = 0;
      ListaArestas[i].descoberta = 0;
      ListaArestas[i].v1 = v1;
      ListaArestas[i].v2 = v2;
    }


	///////////////////////////////////////////////////////////
  //Testando cada algoritmo e exibindo as respostas como 1(true) e 0(false)
  int arvore, conexo, ciclo, floresta;

  clock_t tempo1; //teste de tempo iniciado
	tempo1 = clock();
	
    if(DEBUG_messages==1) printf("Executando: %s\n",argv[2]);
		if(strcmp(argv[2],BuscaGrafo_)==0){
			BuscaGrafo(0, &VerticesVisitados, &ListaArestas, arestas);
		}
    else if (strcmp(argv[2],ImprimeGrafo_)==0){
			ImprimeGrafo(ListaVertice);
		}
    else if (strcmp(argv[2],ExcluiAresta_)==0){
      if(argc>=7){
        ExcluiAresta(&ListaVertice,atoi(argv[5]),atoi(argv[6]));
			}else{  
        switch(VerticesLidos){
          case 5: 
          case 6:
          case 7:
            ExcluiAresta(&ListaVertice,1,2);
            break;
          default:
            ExcluiAresta(&ListaVertice,3,7);
        }

      }
      if(argc>=5) prepare_dump_json(v,a, &ListaArestas, VerticesLidos, arestas, ListaVertice,argv[4],DEBUG_messages);
      
		}
    else if (strcmp(argv[2],ExcluiVertice_)==0){
      if(argc>=6){
          ExcluiVertice(&ListaVertice,atoi(argv[5]),&VerticesLidos);
      }
      else{
          //Exclui o último vertice lido pois é o que mais esforço precisa
			    ExcluiVertice(&ListaVertice,VerticesLidos-1,&VerticesLidos);
      }
      
      if(argc>=5) prepare_dump_json(v,a, &ListaArestas, VerticesLidos, arestas, ListaVertice,argv[4],DEBUG_messages);
      
		}
    else if (strcmp(argv[2],AdicionaAresta_)==0){   
      if(argc>=7){
            AdicionaAresta(&ListaVertice,atoi(argv[5]),atoi(argv[6]));
          }else{  


          switch(VerticesLidos){
          case 5:
          case 10:
            AdicionaAresta(&ListaVertice,2,4);
            break;   
          case 6:
          case 7:
          case 9:
            AdicionaAresta(&ListaVertice,1,4);
            break;      
          case 8:
            AdicionaAresta(&ListaVertice,1,4);
            break;
          case 20:
            AdicionaAresta(&ListaVertice,7,10);
            break;
          case 50:
            AdicionaAresta(&ListaVertice,25,1);
            break;
          case 100:
            AdicionaAresta(&ListaVertice,79,57);
            break;
          case 200:
            AdicionaAresta(&ListaVertice,81,69);
            break;
          default:
            AdicionaAresta(&ListaVertice,31,0);
          }
      }
      if(argc>=5) prepare_dump_json(v,a, &ListaArestas, VerticesLidos, arestas, ListaVertice,argv[4],DEBUG_messages);
     
			
		}
		else if (strcmp(argv[2],BuscaCompleta_)==0){
			BuscaCompleta(&VerticesVisitados, &ListaArestas, arestas, VerticesLidos);
		}
		else if (strcmp(argv[2],EhConexo_)==0){ 
    	conexo = EhConexo(0, &VerticesVisitados, &ListaArestas, VerticesLidos, arestas);
      if(DEBUG_messages==3) printf("Conexo?: %d\n",conexo);
    }
		else if (strcmp(argv[2],EhArvore_)==0){
			arvore = EhArvore(0, &VerticesVisitados, &ListaArestas, VerticesLidos, arestas);
      if(DEBUG_messages==3) printf("Arvore?: %d\n",arvore);
    
		}
		else if (strcmp(argv[2],TemCiclo_)==0){
			ciclo = TemCiclo(&VerticesVisitados, &ListaArestas, VerticesLidos, arestas);
      if(DEBUG_messages==3) printf("Ciclo?: %d\n",ciclo);  
		}
		else if (strcmp(argv[2],EhFloresta_)==0){
			floresta = EhFloresta(&VerticesVisitados, &ListaArestas, VerticesLidos, arestas);
      if(DEBUG_messages==3) printf("Floresta?: %d\n",floresta);
		}
    else if(strcmp(argv[2],ObterFlorestaGeradora_)==0){
      ObterFlorestaGeradora(&VerticesVisitados, &ListaArestas, &FlorestaGeradora, arestas, VerticesLidos);
		}
		else if(strcmp(argv[2],BuscaLargura_)==0){
      BuscaLargura(0, &VerticesVisitados, &ListaArestas, VerticesLidos, arestas, ListaVertice);
		}
    else if(strcmp(argv[2],DeterminarDistancias_)==0){
			DeterminarDistancias(0, &VerticesVisitados, &ListaArestas, VerticesLidos, arestas, ListaVertice, &Dist);
		}
		else if(strcmp(argv[2],BuscaProfundidade_)==0){
      BuscaProfundidade(0, &VerticesVisitados, &ListaArestas, VerticesLidos, arestas, ListaVertice);
		}
    else if(strcmp(argv[2],AdicionaVertice_)==0){
      aux=1;
      InsereVerticeFim(&ListaVertice, VerticesLidos, &VerticesAdicionados);
      if(argc>=5) prepare_dump_json(v,a, &ListaArestas, VerticesLidos, arestas, ListaVertice,argv[4],DEBUG_messages);
		}
    else if(strcmp(argv[2],Vizinhos_)==0){
      if(argc>=5) {          
        ImprimeVizinhosDeVertice(ListaVertice,atoi(argv[4]));
      } 
		}
    else if(strcmp(argv[2],FordFulk_)==0){
      int result = prepare_fordfulk(v, a, c, &ListaArestas, VerticesLidos, arestas, DEBUG_messages, atoi(argv[4]), atoi(argv[5]));
			if(DEBUG_messages==3) printf("O maximo fluxo possivel para o grafo eh: %d\n",result);
      
		}
		
  double toMiliseconds = 1000/CLOCKS_PER_SEC;
  double interval = (clock() - tempo1) * toMiliseconds;
  //float interval = (clock() - tempo1);
  if(DEBUG_messages==1) printf("Tempo de %s: %.3f\n",argv[2],interval);
	if(DEBUG_messages==0) printf("%.3f\n",interval);
  if(DEBUG_messages==4) printf("%d,%.3f\n",VerticesLidos,interval);

  

    //BuscaGrafo(0, &VerticesVisitados, &ListaArestas, arestas);

    //BuscaCompleta(&VerticesVisitados, &ListaArestas, arestas, VerticesLidos);


    //ObterFlorestaGeradora(&VerticesVisitados, &ListaArestas, &FlorestaGeradora, arestas, VerticesLidos);

    //EhConexo(0, &VerticesVisitados, &ListaArestas, VerticesLidos, arestas); 
    
    //TemCiclo(&VerticesVisitados, &ListaArestas, VerticesLidos, arestas);
    
    
    //EhFloresta(&VerticesVisitados, &ListaArestas, VerticesLidos, arestas);
    
    
    //EhArvore(0, &VerticesVisitados, &ListaArestas, VerticesLidos, arestas);

//ImprimeGrafo(ListaVertice);




	  free(a);
    free(ListaArestas);
    free(VerticesVisitados);
    LimpaListaVertice(&ListaVertice);
    LimpaListaVertice(&FlorestaGeradora);
    fclose(Arquivo);
    return 0;
}


struct vizinhos* criaVizinho(int v)
{
    struct vizinhos* p = (struct vizinhos*) malloc(sizeof(struct vizinhos));
    p->valor = v;
    p->prox = NULL;

    return p;
}


struct vertice* criaVertice(int v)
{
    struct vertice* p = (struct vertice*) malloc(sizeof(struct vertice));
    p->valor = v;
    p->prox = NULL;
    p->listavizinhos = NULL;

    return p;
}


void LimpaListaVertice(struct vertice ** p)
{
    if (*p)
    {
        LimpaListaVertice(&((*p)->prox));
        LimpaListaVizinhos(&((*p)->listavizinhos));
        free(*p);
        *p = NULL;
    }
}


void LimpaListaVizinhos(struct vizinhos ** p)
{
    if (*p)
    {
        LimpaListaVizinhos(&((*p)->prox));
        free(*p);
        *p = NULL;
    }
}


void InsereVerticeFim(struct vertice** p, int v, int* N)
{
    if (*p)
    {
        if(DEBUG_messages==3 && aux==1) printf("em elemento = %d\n",(*p)->valor);
        InsereVerticeFim(&((*p)->prox), v, N);
    }

    else
    {
        if(DEBUG_messages==3 && aux==1) printf("Inserindo Vertice no Fim\n");
        struct vertice* elem = criaVertice(v);
        elem->prox = NULL;
        elem->listavizinhos = NULL;
        *p = elem;

        (*N)++;
    }
}


void InsereVizinhoFim(struct vizinhos** p, int v)
{
    if (*p)
    {
        InsereVizinhoFim(&((*p)->prox), v);
    }

    else
    {
        struct vizinhos* elem = criaVizinho(v);
        elem->prox = NULL;
        *p = elem;
    }
}


struct vertice* BuscaVertice(struct vertice* p, int v)
{
    if (p)
    {
        if (v == p->valor)
        {
            //printf("Vertice encontrado %d\n",v);
            return p;
        }

        else
        {
            return BuscaVertice(p->prox, v);
        }
    }

    else
    {
        return NULL;
    }
}


struct vizinhos* BuscaVizinho(struct vizinhos* p, int v)
{
    if (p)
    {
        if (v == p->valor)
        {
            return p;
        }

        else
        {
            return BuscaVizinho(p->prox, v);
        }
    }

    else
    {
        return NULL;
    }
}


void ExcluiVizinho(struct vizinhos** p, int v)
{
    if (*p)
    {
        if (v == (*p)->valor)
        {
            struct vizinhos* e = *p;
            *p = e->prox;
            free(e);
        }

        else
        {
            ExcluiVizinho(&((*p)->prox), v);
        }
    }
}


void ExcluiVertice(struct vertice** p, int v, int* N)
{
    if(*p)
    {
      if(DEBUG_messages==3) printf("ExcluiVertice %d, estou em %d\n",v,(*p)->valor);
      //ExcluiVertice( &((*p)->prox), v, N);
      ExcluiVizinho( &((*p)->listavizinhos), v );
      if((*p)->valor == v)
      {
        if(DEBUG_messages==3) printf("Apagando vertice %d\n",v);
        struct vertice* e = *p;
        *p = e->prox;
        free(e);


        //(*N)--;
      }

      else
      {
        ExcluiVertice( &((*p)->prox), v, N);
      }
    }
}


void ExcluiAresta(struct vertice** p, int u, int v)
{
    struct vertice* VerticeU = BuscaVertice(*p, u);
    struct vertice* VerticeV = BuscaVertice(*p, v);

    ExcluiVizinho( &(VerticeU->listavizinhos), v);
    ExcluiVizinho( &(VerticeV->listavizinhos), u);
}


void AdicionaAresta(struct vertice** p, int u, int v)
{
    struct vertice* VerticeU = BuscaVertice(*p, u);
    struct vertice* VerticeV = BuscaVertice(*p, v);

    //printf("VerticeU %d - VercticeV %d\n",VerticeU->valor, VerticeV->valor);
    InsereVizinhoFim( &(VerticeU->listavizinhos), v);
    InsereVizinhoFim( &(VerticeV->listavizinhos), u);
}


void ImprimeVizinhos(struct vizinhos* p)
{
  if (p)
  {
    printf("Vertice %d capacidade: %d\n", p->valor, p->capacidad);
    ImprimeVizinhos(p->prox);
  }
  else
  {
    printf("Sem mais vizinhos.\n");
  }
}


void ImprimeGrafo(struct vertice* p)
{
  if (p)
  {
    printf("Vizinhos do vertice %d:\n", p->valor);
    ImprimeVizinhos(p->listavizinhos);
    printf("\n\n");
    ImprimeGrafo(p->prox);
  }
}


void ImprimeVizinhosDeVertice(struct vertice* p, int v)
{
  if (p)
  {
    if(p->valor==v){
      printf("Vizinhos do vertice %d\n",v);
      ImprimeVizinhos(p->listavizinhos);
      printf("\n");
    }else{
        ImprimeVizinhosDeVertice(p->prox,v);
    }
  }
}

void BuscaGrafo(int r, int** V, struct ArestaRotulada** E, int m)
{
  (*V)[r] = 1; int i;
  if(DEBUG_messages==3) printf("Visito o vertice %d\n",r);

  for(i = 0; i < m; i++)
  {
    if(DEBUG_messages==3) printf("Vamos da aresta %d, de %d a %d\n",i,(*E)[i].v1,(*E)[i].v2);
    if((*E)[i].explorada == 0 && (*V)[(*E)[i].v1] == 1)
    {
      (*E)[i].explorada = 1;

      if( (*V)[(*E)[i].v2] == 0 )
      {
        if(DEBUG_messages==3) printf("Visito o vertice %d\n",(*E)[i].v2);
        if(DEBUG_messages==3) printf("Descoberta aresta %d-%d\n",(*E)[i].v1,(*E)[i].v2);
        (*V)[(*E)[i].v2] = 1;
        
        (*E)[i].descoberta = 1;
      }
    }

    if((*E)[i].explorada == 0 && (*V)[(*E)[i].v2] == 1)
    {
      (*E)[i].explorada = 1;
      
      if( (*V)[(*E)[i].v1] == 0 )
      {
        if(DEBUG_messages==3) printf("Visito o vertice %d\n",(*E)[i].v1);
        if(DEBUG_messages==3) printf("Descoberta aresta %d-%d\n",(*E)[i].v1,(*E)[i].v2);
        (*V)[(*E)[i].v1] = 1;
        
        (*E)[i].descoberta = 1;
      }
    }
  }
}



void BuscaCompleta(int** V, struct ArestaRotulada** E, int m, int n)
{
  int i;

  for(i = 0; i < m; i++)
  {
    (*E)[i].descoberta = 0;
    (*E)[i].explorada = 0;
  }

  for(i = 0; i < n; i++)
  {
    (*V)[i] = 0;
  }

  for(i = 0; i < n; i++)
  {
    if((*V)[i] == 0)
    {
      BuscaGrafo(i, V, E, m);
    }
  }
}


int EhConexo(int r, int** V, struct ArestaRotulada** E, int n, int m)
{
  BuscaGrafo(r, V, E, m);
  
  int i;
  for(i = 0; i < n; i++)
  {
    if ((*V)[i] == 0)
    {
      if(DEBUG_messages==3) printf("Vertice não visitado: %d\n",i);
      return 0;
    }
  }

  return 1;
}


int TemCiclo(int** V, struct ArestaRotulada** E, int n, int m)
{
  BuscaCompleta(V, E, m, n);

  int i;
  for(i = 0; i < m; i++)
  {
    if((*E)[i].descoberta == 0)
    {
      if(DEBUG_messages==3) printf("Aresta nao descoberta %d-%d\n",(*E)[i].v1,(*E)[i].v2);
      return 1;
    }
  }
  if(DEBUG_messages==3) printf("Todas arestas descobertas.\n");
  return 0;
}


int EhFloresta(int** V, struct ArestaRotulada** E, int n, int m)
{
  if(TemCiclo(V, E, n, m)  == 0)
  {
    return 1;
  }

  else
  {
    return 0;
  }

}


int EhArvore(int r, int** V, struct ArestaRotulada** E, int n, int m)
{
  BuscaGrafo(r, V, E, m);

  int i;
  for(i=0; i < n; i++)
  {
    if((*V)[i] == 0)
    {
      return 0;
    }
  }
  
  for(i = 0; i < m; i++)
  {
    if((*E)[i].descoberta == 0)
    {
      return 0;
    }
  }

  return 1;
}


void ObterFlorestaGeradora(int** V, struct ArestaRotulada** E, struct vertice** T, int m, int n)
{
  BuscaCompleta(V, E, m, n);
  int i; int j = 0;
  for(i = 0; i < m; i++)
  {
    if((*E)[i].descoberta == 1)
    {
      if(DEBUG_messages==3) printf("Adiciono aresta a floresta geradora %d,%d\n", (*E)[i].v1, (*E)[i].v2);
      AdicionaAresta( T, (*E)[i].v1, (*E)[i].v2 );
    }
  }
}


void BuscaLargura(int r, int** V, struct ArestaRotulada** E, int n, int m, struct vertice* p) {
    
    resetFila();
    (*V)[r] = 1;
    entraFila(r);

    while(!filaVazia()){
			int u = sacaFila();
			if(DEBUG_messages==3) printf("Removo o vertice:%d\n",u);
      if(DEBUG_messages==3) printFila();
      struct vertice* VerticeV = BuscaVertice(p, u);
      //printf("--->%d\n",VerticeV->valor);
      struct vizinhos* p = VerticeV->listavizinhos;

      while(p){
        //printf("Vertice %d\n", p->valor);
        int w = p->valor;
        if((*V)[w] == 1){
            if((*E)[w].explorada == 0){
                (*E)[w].explorada =1;
            }
        }else{
            (*E)[w].explorada =1;
            (*E)[w].descoberta =1;
            (*V)[w] = 1;
            if(DEBUG_messages==3) printf("Insiro:%d\n",w);
            entraFila(w);
        }
        p = p->prox;
      }

	}
 //&(VerticeU->listavizinhos)

}



void DeterminarDistancias(int r, int** V, struct ArestaRotulada** E, int n, int m, struct vertice* p, int** D) {
    
    resetFila();
    (*V)[r] = 1;
    entraFila2(r,1);
    (*D)[r] = 0;
    while(!filaVazia()){
			struct par casal = sacaFila2();
			int u = casal.value1;
			int nivel = casal.value2;
			if(DEBUG_messages==3) printf("Removo o vertice:%d, nivel=%d\n",u,nivel);
      if(DEBUG_messages==3) printFila();
      struct vertice* VerticeV = BuscaVertice(p, u);
      //printf("--->%d\n",VerticeV->valor);
      struct vizinhos* p = VerticeV->listavizinhos;

      while(p){
        //printf("Vertice %d\n", p->valor);
        int w = p->valor;
        if((*V)[w] == 1){
            if((*E)[w].explorada == 0){
                (*E)[w].explorada =1;
            }
        }else{
            (*E)[w].explorada =1;
            (*E)[w].descoberta =1;
            (*V)[w] = 1;
            (*D)[w]=nivel;
            if(DEBUG_messages==3) printf("Insiro:%d\n",w);
            entraFila2(w,nivel+1);
        }
        p = p->prox;
      }

	}
/*int i;
for(i=0;i<n;i++){
		printf("%d - ",(*D)[i]);
	}
	printf("\n");*/
}


void BuscaProfundidade(int r, int** V, struct ArestaRotulada** E, int n, int m, struct vertice* p) {

  BuscaProfundidadeVisita(r,V,E,n,m,p);
}


void BuscaProfundidadeVisita(int u, int** V, struct ArestaRotulada** E, int n, int m, struct vertice* verti){
  if(DEBUG_messages==3) printf("Visita:%d\n",u);
	(*V)[u] = 1;
  struct vertice* VerticeV = BuscaVertice(verti, u);
  struct vizinhos* p = VerticeV->listavizinhos;
  while(p){
        //printf("Vertice %d\n", p->valor);
        int w = p->valor;
        if((*V)[w] == 1){
            if((*E)[w].explorada == 0){
                (*E)[w].explorada =1;
            }
        }else{
            (*E)[w].explorada =1;
            (*E)[w].descoberta =1;
            BuscaProfundidadeVisita(w,V,E,n,m,verti);
        }
        p = p->prox;
  }
}

void prepare_dump_json(int *ve,int **ar,struct ArestaRotulada** E, int n, int m, struct vertice* p,char *fileName, int debug){
	int i, j;
	int aresta = 0;
  int vertice = 0;
  while(p){
    if(DEBUG_messages==3) printf("Revisando vertice %d\n",p->valor);
    int v =  p->valor;
    vertice++;
    struct vizinhos* q = p->listavizinhos;    
    while(q){
       int w = q->valor;
       if(w>v){//nao foi considerado previamente
          ar[aresta][0]=v;
          ar[aresta][1]=w;
          aresta++;
       }
       q = q->prox;
    }
    p = p->prox;
  }

  int *vertices = &vertice;
  int *arestas = &aresta;
	if(debug==3) printf("Revisados: %d vertices e %d arestas.\n",*vertices,aresta);
	dump_json(ve,ar,vertices,arestas,fileName,debug);	
}


void InsereVizinhoFimCapacidad(struct vizinhos** p, int v, int capacidad)
{
    if (*p)
    {
        InsereVizinhoFimCapacidad(&((*p)->prox), v,capacidad);
    }

    else
    {
        struct vizinhos* elem = criaVizinho(v);
        elem->capacidad = capacidad;
        elem->prox = NULL;
        *p = elem;
    }
}


void AdicionaArestaDireccionado(struct vertice** p, int u, int v, int c)
{
    //Somente incluimos o vizinho no vertice u porque é grafo direccionado
    struct vertice* VerticeU = BuscaVertice(*p, u);
    
    //Mas, existe já essa aresta com por exemplo valor 0 de outra inversa? 
    struct vizinhos* vizinhoU= BuscaVizinho(VerticeU->listavizinhos,v);
    if(vizinhoU==NULL){
     
      InsereVizinhoFimCapacidad( &(VerticeU->listavizinhos), v, c);
    }else{
     
      vizinhoU->capacidad = c;
    }

    //Para a aresta inversa, a criamos mas colocamos capacidade 0, pois o algoritmo precisa atualizar esta 
    //capacidade inversa
    //Mas, existe já essa aresta?
    struct vertice* VerticeV = BuscaVertice(*p, v);
    struct vizinhos* vizinhoV= BuscaVizinho(VerticeV->listavizinhos,u);
    if(vizinhoV==NULL){
     
      InsereVizinhoFimCapacidad( &(VerticeV->listavizinhos), u, 0);
    }else{
     
      //Não fazemos nada pois iamos colocar um zero.
    }
    
   
}

/* Retorna 1 se existe um caminho desde fonte s até fonte t no grafo residual.  
 também preenche o pai (parent[]) para guardar o caminho */
int BuscaLarguraFord(struct vertice* ListaVerticeDir, int s, int t, int parent[], int vertices) 
{ 
  if(DEBUG_messages==3) printf("Busca?: s=%d t=%d\n",s,t);

  int visited[vertices]; 
  memset(visited, 0, sizeof(visited)); 
  

	resetFila();
	entraFila(s);
	
  visited[s] = 1; 
  parent[s] = -1; 

  // Busca Por largura estandar acrescentando os detalhes da capacidade e o pai (parent) 
  while (!filaVazia()) 
  { 
  int u = sacaFila();
  	  //if(DEBUG_messages==3) printf("Removo o vertice:%d\n",u);
      //if(DEBUG_messages==3) printFila();
      struct vertice* VerticeU = BuscaVertice(ListaVerticeDir, u);
      //printf("--->%d\n",VerticeV->valor);
      struct vizinhos* p = VerticeU->listavizinhos;

      while(p){
        //printf("Vertice %d\n", p->valor);
        int w = p->valor;
        //printf("%d -> %d :capacidad %d\n",u,w,p->capacidad);
        if(visited[w]==0 && p->capacidad>0){
            visited[w] = 1;
            parent[w] = u;  
            //if(DEBUG_messages==3) printf("Insiro:%d\n",w);
            entraFila(w);
        }
        p = p->prox;
      }
  }
  // Se o fim (o vertice t) foi visitado começando desde a fonte (s), então retorna  
  // 1, senão 0 
  return (visited[t] == 1); 

}

int min(int a, int b){
	if (a>b) return b;
	else return a;
}

// Retorna o fluxo máximo desde s a t para o grafo 
int fordFulkerson(struct vertice* ListaVerticeDir, int s, int t, int debug, int vertices, int arestas) 
{ 
    int u, v, i, j; 
    int parent[vertices];  // Array que sera preenchido por BuscaLargura Ford para guardar o caminho 
    int max_flow = 0;  // Não tem fluxo inicialmente
  
   int count = 0;
    // Aumenta o fluxo enquanto existe um caminho da fonte (s) até o fim (t) 
    while (BuscaLarguraFord(ListaVerticeDir, s, t, parent, vertices)) 
    { 
        // Achar a capacidade minimal residual das arestas ao longo do 
        // caminho preenchido por BuscaLarguraFord. 
        int path_flow = INT_MAX; 
        for (v=t; v!=s; v=parent[v]) 
        { 
            u = parent[v]; 
            struct vertice* verticeU = BuscaVertice(ListaVerticeDir,u);
            struct vizinhos* vizinhoV= BuscaVizinho(verticeU->listavizinhos,v);
            if(DEBUG_messages==3) printf("min: %d %d u=%d v=%d\n",path_flow,vizinhoV->capacidad,u,v);
            path_flow = min(path_flow, vizinhoV->capacidad); 
        } 
        if(DEBUG_messages==3) printf("path_flow: %d \n",path_flow);
        // atualizamos a capacidades residuais das arestas e arestas reversas
        // ao longo do caminho
        for (v=t; v != s; v=parent[v]) 
        { 
            u = parent[v];
            struct vertice* verticeU = BuscaVertice(ListaVerticeDir,u);
            struct vizinhos* vizinhoV= BuscaVizinho(verticeU->listavizinhos,v); 
            if(DEBUG_messages==3) printf("Capacidade de %d-%d: %d - %d \n",u,v,vizinhoV->capacidad,path_flow);
            vizinhoV->capacidad -= path_flow; 
              
            struct vertice* verticeV = BuscaVertice(ListaVerticeDir,v);
            struct vizinhos* vizinhoU= BuscaVizinho(verticeV->listavizinhos,u); 
            if(DEBUG_messages==3) printf("Capacidade de %d-%d: %d + %d \n",v,u,vizinhoU->capacidad,path_flow);
            vizinhoU->capacidad += path_flow; 
            
        } 
  
        // Acrecentamos o caminho do fluxo ao fluxo total
        max_flow += path_flow; 
        count++;
        if(count>4200000){
          printf("Superado numero maximo de iterações.\n");  
          break;
        }
    } 
   
    // Retornamos o fluxo total
    return max_flow; 

}

int prepare_fordfulk(int *ve,int **ar,int *ca, struct ArestaRotulada** E, int vertices, int arestas, int debug, int s, int t){
	//Criamos uma estrutura adicional para um grafo direcionado com lista, independente do grafo já existente
  //para os outros métodos
  int i;
  int aux;
  struct vertice* ListaVerticeDir = NULL;
  //printf("vertices %d\n",vertices);
  
  
  for(i = 0; i < vertices; i++)
    {
      InsereVerticeFim(&ListaVerticeDir, i, &aux);
    }
    
    for(i = 0; i < arestas; i++)
    {
      
      AdicionaArestaDireccionado(&ListaVerticeDir, (*E)[i].v1, (*E)[i].v2, ca[i]);
      
    }
    

	//ImprimeGrafo(ListaVerticeDir);
	return fordFulkerson(ListaVerticeDir, s, t, debug, vertices,arestas); 
	
}

