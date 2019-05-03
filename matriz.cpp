#include <stdio.h>
#include <stdlib.h>

int **AdicionaVertice(int **, int *);
int **RemoveVertice(int **, int *, int);
void RemoveAresta(int **, int, int);
void AdicionaAresta(int **, int, int);
void Vizinhos(int **, int, int);
void ImprimeMatriz(int **, int);


int main() {
	
//	FILE *Arquivo = NULL;
	int **Matriz = NULL;
	int vertices = 4;
	int arestas = 3;
	int i, j, v1, v2;
	
//	Arquivo = fopen("grafo.txt", "r");
//	fscanf(Arquivo, "%d\t%d", &vertices, &arestas);
	
	Matriz = (int **)malloc(vertices * sizeof(int *));
	for (i = 0; i < vertices; i++) {
		Matriz[i] = (int *)malloc(vertices * sizeof(int));
	}

	for (i = 0; i < vertices; i++) {
		for (j = 0; j < vertices; j++) {
			Matriz[i][j] = 0;
		}
	}

	Matriz[0][3] = 1;
	Matriz[1][3] = 1;
	Matriz[2][3] = 1;
	Matriz[3][0] = 1;
	Matriz[3][1] = 1;
	Matriz[3][2] = 1;
	
	for (i = 0; i < vertices; i++) {
		for (j = 0; j < vertices; j++) {
			printf("%d ", Matriz[i][j]);
			if (j == vertices-1)
			{
				printf("\n");
			}
		}
	}
	printf("\n");
	
	ImprimeMatriz(Matriz, vertices);
	Vizinhos(Matriz, vertices, 3);
	
	for (i = 0; i < vertices; i++) {
		free(Matriz[i]);
	}

	free(Matriz);

//	fclose(Arquivo);

	return 0;
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

	printf("\n");
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

	printf("Lista de nos vizinhos do vertice %d:\n", v);

	for (i = 0; i < n; i++) {
		if (Matriz[v][i]) {
			printf("Vertice %d\n", i);
			TemVizinho = 1;
		}
	}

	if (TemVizinho == 0) {
		printf("Nao ha nos vizinhos.\n");
	}
}

int **AdicionaVertice(int **Matriz, int *N) {
	int **MatrizNova = NULL;
	int i, j;
	int Vertices = (*N);
	int NovoVertices = (*N) + 1;

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

	for (i = 0; i < Vertices; i++) {
		free(Matriz[i]);
	}

	(*N)++;

	return MatrizNova;
}

int **RemoveVertice(int **Matriz, int *N, int V) {
	int **MatrizNova = NULL;
	int i, j;
	int Vertices = (*N);
	int NovoVertices = (*N) - 1;

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

	for (i = 0; i < Vertices; i++) {
		free(Matriz[i]);
	}

	(*N)--;

	return MatrizNova;
}
