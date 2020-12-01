// verifica o arquivo json

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"

int main(int argc,char* argv[])
{
    FILE* Arquivo = NULL;
    int LIMITE_ARESTAS = 400000;
    int v[1000];
    int json_ = 0;
    int VerticesLidos = 0;
    int arestas = 0; 
    int **a = NULL;
      a = (int **)malloc(LIMITE_ARESTAS * sizeof(int *));
      int i_;
      for (i_ = 0; i_ < LIMITE_ARESTAS; i_++) {
      a[i_] = (int *)malloc(2 * sizeof(int));
    }
    parse_json(v,a,&VerticesLidos,&arestas,argv[1],0);
    printf("%d\t%d\t%d\n",VerticesLidos,arestas,VerticesLidos+arestas);
    return 0;
}
