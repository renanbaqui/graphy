#include <stdio.h>
#include <stdlib.h>

struct vizinhos
{
    int valor;
    struct vizinhos* prox;
};


struct vertice
{
    int valor;
    struct vertice* prox;
    struct vizinhos* listavizinhos;
};

int main()
{
//	FILE* Arquivo = NULL;	 
	struct vertice* ListaVertice = NULL;
    int VerticesLidos = 0; int arestas = 0; int i, j, v1, v2;    

    for(i = 0; i < VerticesLidos; i++)
    {
            
    }

    for(i = 0; i < arestas; i++)
    {
//		fscanf(Arquivo, "%d\t%d", &v1, &v2);
//     	AdicionaAresta(&ListaVertice, v1, v2);      
    }
   
   
//	fclose(Arquivo);
    return 0;
}






















