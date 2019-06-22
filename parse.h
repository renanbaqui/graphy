#ifndef PARSE_H_   /* Include guard */
#define PARSE_H_

void printVertices(int vert[], int n);
void printArestas(int arestas[][2], int n);
int parse_json(int *ve,int **ar,int *vertices, int *arestas,char *fileName, int debug);
int parse_json_capacity(int *ve,int **ar, int *cap, int *vertices, int *arestas,char *fileName, int debug);
int dump_json(int *ve,int **ar,int *vertices, int *arestas,char *fileName, int debug);
//int ar[][2]
#endif // PARSE_H_